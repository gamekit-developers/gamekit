
//  Known problems:
//  - there is very little user code configurability
//  - some limitations in the protocol are arbitrary (name length, record length, etc)
//  - the protocol allows you to make me send packets to a third party, if you can 
//    forge the source IP of your outgoing traffic
//  - there is no encryption of login data (or any other data)
//  - there is no authentication of traffic once logged in (except by source IP)

#include "roster.h"
#include "message.h"
#include "sort_addr.h"

#include <assert.h>
#include <list>
#include <map>
#include <string>
#include <vector>
#include <algorithm>

#define NAME_LEN 64
#define PEER_TIMEOUT_SECONDS 30
#define MAX_RECORD_SIZE 512
#define MAX_RECORDS_PER_PEER 8
#define MAX_PEERS 128
#define MAX_MATCHED_RECORDS 32
#define CONNECT_RETRY_SECONDS 3
#define KEY_RESEND_SECONDS 3
#define INTRODUCTION_RESEND_SECONDS 0.8
#define PEER_ROSTER_TIMEOUT 30
#define PEER_KEEPALIVE_INTERVAL 7

//  Timeout in seconds to get a query operation through to the server
#define SIMPLE_TIMEOUT_SECONDS 5
#define QUERY_RESEND_SECONDS 2


using namespace libintroduce;

//  Record packet:
//  ID
//  string

//  Record ack packet:
//  ID

//  Query packet:
//  ID
//  string

//  Each Answer packet:
//  ID
//  total num
//  this index
//  score
//  string
//  sockaddr_in

namespace {
  class C_Roster : public I_Roster, public I_SocketUser, public I_SocketPeerUser {
  public:
    C_Roster();
    ~C_Roster();

    bool configure( I_Clock * clock, I_Socket * socket, I_RosterUser * user, bool ownSocket );
    
    //  I_Roster
    void dispose();
    I_Clock * clock();
    I_Network * network();
    I_Socket * socket();
    I_RosterUser * rosterUser();
    void step();
    bool addRecord( sockaddr_in const & peer, char const * record, int token );
    void removeRecords( sockaddr_in const & peer );
    int matchRecords( char const * key, MatchedRecord * outMatches, 
        int maxMatches, float threshold );
    void introduce( sockaddr_in const & peer1, sockaddr_in const & peer2 );

    //  I_SocketUser
    void disposing( I_Socket * socket );
    void onConnection( I_Socket * socket, sockaddr_in const & addr, 
        char const * buf, int bSize );

    //  I_SocketPeerUser
    void disposing( I_SocketPeer * peer );
    void onMessage( I_SocketPeer * peer, char const * buf, int bSize );

    I_Clock * clock_;
    I_Socket * socket_;
    bool ownSocket_;
    I_RosterUser * user_;

    //  The list of peers is sorted in timeout order, low to high.
    //  Any peer in the list has been validated with a valid user name and password.
    struct Peer {
      I_SocketPeer * peer;
      double timeout;
      double keepalive;
    };
    std::list< Peer > peers_;

    //  a Record is what peers ask me to store and match against
    struct Record {
      int token;
      char data[ MAX_RECORD_SIZE ];
    };
    typedef std::map< sort_sockaddr_in, std::list< Record > > RecordMap;
    RecordMap records_;

    void assertPeerListSorted() {
  #if !defined( NDEBUG )
      std::list< Peer >::iterator i1, i2;
      i1 = peers_.begin();
      i2 = peers_.begin();
      while( i1 != peers_.end() ) {
        assert( (*i1).timeout >= (*i2).timeout );
        i2 = i1;
        ++i1;
      }
  #endif
    }

    //! \f validatePacket() will strip the \m bigEndian_ flag from \m ph.type if present 
    //! and correct. It will not validate the packet if it's not what I expect. This means 
    //! that I correctly reject, but cannot interoperate with, a host of another endian-
    //! ness. This could obviously be betterized in the future.
    bool validatePacket( char const * buf, int bSize, char const * func, ProtoHeader & ph );
    void sendConnectAckToPeer( I_SocketPeer * peer );
    void sendKickToAddress( sockaddr_in const & addr );
    void keepalive( I_SocketPeer * peer );
    void sendRecordAckToAddress( sockaddr_in const & addr, int token );
    void sendAnswerToPeer( int id, int n, MatchedRecord const * matches, I_SocketPeer * peer );
    void debugMessage( char const * ptr, size_t size, sort_sockaddr_in ssin );
    void sendKeepaliveToPeer( C_Roster::Peer * peer, double now );

    static double peerTimeout_;
    static unsigned char bigEndian_;
    static int maxRecordsPerPeer_;
    static int maxPeers_;
  };
}

double C_Roster::peerTimeout_ = PEER_TIMEOUT_SECONDS;
unsigned char C_Roster::bigEndian_ = ((htons(1)==1) ? TypeBigEndian : 0);
int C_Roster::maxRecordsPerPeer_ = MAX_RECORDS_PER_PEER;
int C_Roster::maxPeers_ = MAX_PEERS;

//  If you create me with a network and port, I will create my own socket, 
//  and I will dispose it when I'm disposed.
I_Roster * libintroduce::CreateRoster( I_Clock * clock, I_Network * network, unsigned short port, I_RosterUser * user )
{
  if( !network || !clock || !port || !user ) {
    warning( "CreateRoster(network) was passed bad parameters" );
    return 0;
  }

  C_Roster * r = new C_Roster();

  I_Socket * sock = network->createSocket( port, r );
  if( !sock ) {
    warning( "Could not create socket in CreateRoster(port %d)", port );
    r->dispose();
    return 0;
  }

  if( !r->configure( clock, sock, user, true ) ) {
    warning( "Could not configure roster after creation" );
    r->dispose();
    return 0;
  }

  return static_cast< I_Roster * >( r );
}

//  If you create me with a socket, I will not dispose that socket when disposed
I_Roster * libintroduce::CreateRoster( I_Clock * clock, I_Socket * socket, I_RosterUser * user )
{
  if( !clock || !socket || !user ) {
    warning( "CreateRoster(socket) was passed bad parameters" );
    return 0;
  }

  C_Roster * r = new C_Roster();

  if( !r->configure( clock, socket, user, false ) ) {
    warning( "Could not configure roster with provided socket after creation" );
    r->dispose();
    return 0;
  }
  else {
    socket->setSocketUser( r );
  }

  return static_cast< I_Roster * >( r );
}



C_Roster::C_Roster()
{
  socket_ = 0;
  ownSocket_ = false;
  user_ = 0;
}

C_Roster::~C_Roster()
{
  assert( !ownSocket_ );
  assert( user_ == 0 );
  assert( peers_.size() == 0 );
}

bool C_Roster::configure( I_Clock * clock, I_Socket * socket, I_RosterUser * user, bool ownSocket )
{
  if( socket_ || user_ ) {
    return false;
  }
  clock_ = clock;
  socket_ = socket;
  user_ = user;
  ownSocket_ = ownSocket;
  return true;
}

void C_Roster::dispose()
{
  debug( "C_Roster::dispose() 0x%lx ownSocket_ %s\n", this, ownSocket_ ? "true" : "false" );

  if( user_ ) {
    user_->disposing( this );
    user_ = 0;
  }

  if( ownSocket_ ) {
    socket_->dispose();
  }
  assert( !ownSocket_ );

  std::list< Peer >::iterator ptr = peers_.begin();
  while( ptr != peers_.end() ) {
    (*ptr).peer->dispose();
    ++ptr;
  }
  peers_.clear();

  delete this;
}

I_Clock * C_Roster::clock()
{
  return clock_;
}

I_Network * C_Roster::network()
{
  return socket_->network();
}

I_Socket * C_Roster::socket()
{
  return socket_;
}

I_RosterUser * C_Roster::rosterUser()
{
  return user_;
}

void C_Roster::step()
{
  assertPeerListSorted();

  double now = clock_->realTime();

  //  expire peers that have been inactive too long
  std::list< Peer >::iterator p = peers_.begin();
  while( p != peers_.end() ) {
    if( (*p).timeout > now ) {
      //  there won't be any more peers to expire right now
      break;
    }
    debug( "C_Roster::step() 0x%lx find peer 0x%lx timeout\n", this, (*p).peer );
    //  time out this peer
    std::list< Peer >::iterator del = p;
    ++p;
    I_SocketPeer * peer = (*del).peer;
    peers_.erase( del );
    user_->userTimeout( this, peer->peerAddress() );
    sendKickToAddress( peer->peerAddress() );
    peer->dispose();
  }
  while( p != peers_.end() ) {
    if( (*p).keepalive < now ) {
      sendKeepaliveToPeer( &(*p), now );
    }
    ++p;
  }

  assertPeerListSorted();
}

void C_Roster::sendKeepaliveToPeer( C_Roster::Peer * peer, double now )
{
  peer->keepalive = now + PEER_KEEPALIVE_INTERVAL;
  
  debug( "C_Roster::sendKeepaliveToPeer() 0x%lx peer 0x%lx now %g\n",
      this, peer->peer, now );

  Message mm;
  ProtoHeader::prepend( Keepalive | bigEndian_, mm );
  peer->peer->sendMessage( mm.buf_, mm.size_ );
}

bool C_Roster::addRecord( sockaddr_in const & peer, char const * record, int token )
{
  sort_sockaddr_in ssin( peer );
  debug( "C_Roster::addRecord() 0x%lx peer %s token %d '%s'\n", this, ssin.staticText(), token, record );

  Record r;
  strncpy( r.data, record, sizeof( r.data ) );
  r.data[ sizeof( r.data )-1 ] = 0;
  r.token = token;
  std::list< Record > & recs = records_[ peer ];
  for( std::list< Record >::iterator ptr = recs.begin();
      ptr != recs.end();
      ++ptr ) {
    if( (*ptr).token == token ) {
      *ptr = r;
      goto added;
    }
  }
  if( int(records_[ peer ].size()) >= maxRecordsPerPeer_ ) {
    return false;
  }
  records_[ peer ].push_back( r );
added:
  sendRecordAckToAddress( peer, token );
  return true;
}

void C_Roster::removeRecords( sockaddr_in const & peer )
{
  sort_sockaddr_in ssin( peer );
  debug( "C_Roster::removeRecords() 0x%lx peer %s\n", this, ssin.staticText() );

  RecordMap::iterator ptr = records_.find( peer );
  if( ptr != records_.end() ) {
    records_.erase( ptr );
  }
}

int C_Roster::matchRecords( char const * key, MatchedRecord * outMatches, 
    int maxMatches, float threshold )
{
  //  I don't sort the matches; the N first that match over threshold are 
  //  what I return.
  RecordMap::iterator ptr = records_.begin();
  int matched = 0;
  while( ptr != records_.end() && maxMatches > 0 ) {
    for( std::list< Record >::iterator l = (*ptr).second.begin(); 
        l != (*ptr).second.end() && maxMatches > 0; 
        ++l ) {
      float m = MatchRecord( (*l).data, key );
      if( m >= threshold ) {
        outMatches->addr = (*ptr).first;
        outMatches->record = (*l).data;
        outMatches->score = m;
        ++matched;
        ++outMatches;
        --maxMatches;
      }
    }
    ++ptr;
  }
  return matched;
}

void C_Roster::introduce( sockaddr_in const & peer1, sockaddr_in const & peer2 )
{
  //  maybe I should check that the two addresses really are peer addresses
  Message m;
  m.put( peer2 );
  ProtoHeader::prepend( IntroduceAck, m );
  debugMessage( m.buf_, m.size_, peer1 );
  socket_->sendMessage( peer1, m.buf_, m.size_ );
  m.clear();
  m.put( peer1 );
  ProtoHeader::prepend( IntroduceAck, m );
  debugMessage( m.buf_, m.size_, peer2 );
  socket_->sendMessage( peer2, m.buf_, m.size_ );
}

void C_Roster::disposing( I_Socket * socket )
{
  assert( socket == socket_ );
  ownSocket_ = false;
  socket_ = 0;
}

void C_Roster::onConnection( I_Socket * socket, sockaddr_in const & addr, 
    char const * buf, int bSize )
{
  ProtoHeader ph;
  if( !validatePacket( buf, bSize, "C_Roster::onConnection()", ph ) ) {
    return;
  }

  debug( "C_Roster::onConnection() 0x%lx type %d\n", this, ph.type );

  if( ph.type != Connect ) {
    warning( "unexpected packet (type %d) in C_Roster::onConnection()", ph.type );
    //  Maybe he didn't get the message the last time?
    //  Send a message without a peer.
    sendKickToAddress( addr );
    return;
  }
  
  char name[ NAME_LEN ];
  char password[ NAME_LEN ];
  Message m( buf+sizeof( ProtoHeader ), bSize-sizeof( ProtoHeader ) );
  if( !m.get( name, NAME_LEN ) ) {
    warning( "could not get user name from packet in C_Roster::onConnection()" );
    return;
  }
  //  The password is sent in clear text!
  if( !m.get( password, NAME_LEN ) ) {
    warning( "could not get password from packet in C_Roster::onConnection()" );
    return;
  }
  if( (int(peers_.size()) < maxPeers_) && user_->validatePeer( this, addr, name, password ) ) {
    //  make a note of this user
    I_SocketPeer * peer = socket->createPeer( addr, this );
    Peer p;
    p.peer = peer;
    double now = clock_->realTime();
    p.timeout = now + peerTimeout_;
    p.keepalive = now;
    peers_.push_back( p );

    //  send back a connection ack
    sendConnectAckToPeer( peer );
  }
  // else say nothing

  assertPeerListSorted();
}

void C_Roster::disposing( I_SocketPeer * peer )
{
  removeRecords( peer->peerAddress() );
}

void C_Roster::onMessage( I_SocketPeer * peer, char const * buf, int bSize )
{
  //  This chunk of packet validation code could be common with onConnection()
  ProtoHeader ph;
  if( !validatePacket( buf, bSize, "C_Roster::onMessage()", ph ) ) {
    return;
  }

  debug( "C_Roster::onMessage() 0x%lx type %d\n", this, ph.type );

  Message m( buf+sizeof( ProtoHeader ), bSize-sizeof( ProtoHeader ) );

  switch( ph.type ) {
  case Connect: {
      //  Send back a connection ack. Because it's onMessage(), I know the peer 
      //  has already been identified with user name and password, although I 
      //  don't use any cryptography to assert that identity.
      sendConnectAckToPeer( peer );
      keepalive( peer );
    }
    break;
  case Disconnect: {
    }
    break;
  case Keepalive: {
      keepalive( peer );
    }
    break;
  case ::Record: {
      char record[ MAX_RECORD_SIZE ];
      int id;
      if( m.get( &id ) && m.get( record, sizeof( record ) ) ) {
        //todo: should this return a bool, and the ack be sent from this place 
        //  instead of inside the registration?
        user_->registerRecord( this, peer->peerAddress(), record, id );
      }
      else {
        warning( "bad packet format in Record request in C_Roster::onMessage()" );
      }
    }
    break;
  case Query: {
      char key[ MAX_RECORD_SIZE ];
      int id;
      MatchedRecord matches[MAX_MATCHED_RECORDS];
      if( m.get( &id ) && m.get( key, sizeof( key ) ) ) {
        int n = user_->matchRecords( this, peer->peerAddress(), key, id, 
            matches, sizeof(matches)/sizeof(matches[0]) );
        sendAnswerToPeer( id, n, matches, peer );
      }
      else {
        warning( "bad packet format in Query request in C_Roster::onMessage()" );
      }
    }
    break;
  case IntroducePeer: {
      // I know that I'm already connected here
      sockaddr_in sin;
      if( m.get( &sin ) ) {
        user_->allowIntroduction( this, peer->peerAddress(), sin );
      }
      else {
        warning( "bad packet format in IntroducePeer request in C_Roster::onMessage()" );
      }
    }
    break;
  default:
    warning( "unexpected packet (type %d) in C_Roster::onMessage()", ph.type );
    return;
  }  
}

bool C_Roster::validatePacket( char const * buf, int bSize, char const * func, ProtoHeader & ph )
{
  if( bSize < sizeof( ProtoHeader ) ) {
    warning( "short packet (%d bytes) in %s", bSize, func );
    return false;
  }
  memcpy( &ph, buf, sizeof( ph ) );
  if( (ph.type & TypeBigEndian) != bigEndian_ ) {
    warning( "wrong endian order or mal-formed packet (type %d) in %s", ph.type, func );
    return false;
  }
  ph.type &= ~TypeBigEndian;
  if( bSize != sizeof( ProtoHeader ) + ph.size ) {
    warning( "packet length calculated wrong (is %d should be %d) in %s",
        ph.size, bSize - sizeof( ProtoHeader ), func );
    return false;
  }
  return true;
}

void C_Roster::sendConnectAckToPeer( I_SocketPeer * peer )
{
  log( "C_Roster::sendConnectAckToPeer() 0x%lx peer 0x%lx addr %s\n", 
      this, peer, sort_sockaddr_in( peer->peerAddress() ).staticText() );

  ProtoHeader ack;
  ack.type = ConnectAck | bigEndian_;
  ack.size = 0;
  //  can't get status back, because it's unreliable
  peer->sendMessage( &ack, sizeof( ack ) );
}

void C_Roster::sendKickToAddress( sockaddr_in const & addr )
{
  log( "C_Roster::sendKickToAddress() 0x%lx addr %s\n", 
      this, sort_sockaddr_in( addr ).staticText() );

  //  The message contains a reason for kicking, as a string.
  Message m;
  m.put( "You are not logged in." );
  ProtoHeader::prepend( Kick | bigEndian_, m );
  socket_->sendMessage( addr, m.buf_, m.size_ );
}

void C_Roster::keepalive( I_SocketPeer * peer )
{
  //  Find the record for the peer, update the timeout, 
  //  and put it back in the list in the appropriate spot.
  //  This spot is likely at the end of the list, but if 
  //  there are different time-outs for different clients, 
  //  that's not necessarily true.
  for( std::list< Peer >::iterator ptr = peers_.begin();
      ptr != peers_.end();
      ++ptr ) {
    if( (*ptr).peer == peer ) {
      Peer p = *ptr;
      ptr = peers_.erase( ptr );
      double nuTime = clock_->realTime() + peerTimeout_;
      debug( "C_Roster::keepalive() 0x%lx peer 0x%lx updating time from %g to %g\n", 
          this, p.peer, p.timeout, nuTime );
      p.timeout = nuTime;
      while( ptr != peers_.end() && ((*ptr).timeout <= p.timeout) ) {
        ++ptr;
      }
      peers_.insert( ptr, p );
      break;
    }
  }
}

void C_Roster::sendRecordAckToAddress( sockaddr_in const & addr, int token )
{
  debug( "C_Roster::sendRecordAckToAddress() 0x%lx token %d to %s\n",
      this, token, sort_sockaddr_in( addr ).staticText() );

  Message mm;
  mm.put( token );
  ProtoHeader::prepend( RecordAck | bigEndian_, mm );
  socket_->sendMessage( addr, mm.buf_, mm.size_ );
}

void C_Roster::sendAnswerToPeer( int id, int n, MatchedRecord const * matches, I_SocketPeer * peer )
{
  debug( "C_Roster::sendAnswerToPeer() 0x%lx id %d with %d records (peer 0x%lx = %s)\n",
      this, id, n, peer, sort_sockaddr_in( peer->peerAddress() ).staticText() );

  //  Because each packet can't be bigger than 255 bytes (ProtoHeader)
  //  I have to send a number of packets.
  //  There's also a limit to the answer size.
  //  If we have no answers at all, we should say so.
  for( int ix = 0; ix < n || ix == 0; ++ix ) {
    Message mm;
    mm.put( id );
    mm.put( n );
    mm.put( ix );
    if( n == 0 ) {
      mm.put( (float)0 );
      mm.put( "" );
      sockaddr_in junk;
      memset( &junk, 0, sizeof( junk ) );
      mm.putn( sizeof( junk ), &junk );
    }
    else {
      mm.put( matches[ix].score );
      mm.put( matches[ix].record );
      mm.putn( sizeof( matches[ix].addr ), &matches[ix].addr );
    }
    ProtoHeader::prepend( Answer, mm );
    peer->sendMessage( mm.buf_, mm.size_ );
  }
}

void C_Roster::debugMessage( char const * buf, size_t size, sort_sockaddr_in ssin )
{
#if !defined( NDEBUG )
  //  create a hex dump of the message and log, in addition to destination
  char msg[1024];
  char * ptr = msg;
  size_t done = 0;
  while( done < size ) {
    //  we can add about 3+3+16+1 characters in one iteration
    if( ptr - msg > 950 ) {
      sprintf( ptr, " ..." );
      break;
    }
    if( !(done & 0xf) ) {
      sprintf( ptr, "%04x  ", done );
      ptr += strlen( ptr );
    }
    sprintf( ptr, "%02x ", buf[done] );
    ptr += 3;
    if( (done & 0xf) == 0xf ) {
      sprintf( ptr, "   " );
      ptr += 3;
      for( int i = -15; i <= 0; ++i ) {
        if( buf[done+i] < 32 || buf[done+i] > 127 ) {
          *ptr++ = '.';
        }
        else {
          *ptr++ = buf[done+i];
        }
      }
      sprintf( ptr, "\n" );
      ptr++;
    }
    ++done;
  }
  *ptr = 0;
  if( ptr > msg && ptr[-1] != '\n' ) {
    sprintf( ptr, "\n" );
  }

  debug( "C_Roster::debugMessage() 0x%lx addr %s\n%s", this, ssin.staticText(), msg );
#endif
}


namespace {
  class C_RosterQuery : public I_RosterQuery, public I_SocketUser, public I_SocketPeerUser {
    public:
      struct SQuery;
      C_RosterQuery();
      ~C_RosterQuery();
      void disconnectSync();
      void connectPeer( I_Socket * socket, I_SocketPeer * peer, 
          char const * name, char const * password );
      void maybeSendConnect( double time ); 
      void onAnswer( I_SocketPeer * peer, Message & m );
      void maybeNotifyUser( int id, SQuery & a );
      void maybeSendQueries( double time );
      void maybeSendKeys( double time );
      void onRecordAck( I_SocketPeer * peer, Message & m );
      void maybeSendIntroductions( double curTime );
      void onIntroduceAck( I_SocketPeer * peer, Message & m );
      void maybeTimeoutRoster( double curTime );

      //  I_RosterQuery
      void dispose();
      void connect( I_Socket * socket, char const * server, 
          char const * name, char const * password );
      void connect( I_Socket * socket, sockaddr_in const & server, 
          char const * name, char const * password );
      void notify( I_RosterConnectionNotify * notify );
      bool connected();
      void step( double now );
      void query( char const * key, I_RosterQueryUser * user, 
          int maxRecords );
      void add( char const * key, int recId );
      void introduce( sockaddr_in const & sin );

      //  I_SocketUser
      void disposing( I_Socket * socket );
      void onConnection( I_Socket * socket, sockaddr_in const & addr, 
          char const * buf, int bSize );

      //  I_SocketPeerUser
      void disposing( I_SocketPeer * peer );
      void onMessage( I_SocketPeer * peer, char const * buf, int bSize );

      I_Socket * socket_;
      I_SocketPeer * serverPeer_;
      sort_sockaddr_in server_;
      std::string userName_;
      std::string userPassword_;
      I_RosterConnectionNotify * notify_;
      bool connected_;
      double lastConnectTime_;
      double lastRosterMessage_;
      double lastTime_;

      struct SAnswer {
        SAnswer() : ok_( false ) {}
        bool ok_;
        std::string data_;
        sockaddr_in addr_;
        float score_;
      };

      struct SQuery {
        SQuery() {}
        SQuery( int i, char const * k, I_RosterQueryUser * u ) :
            id( i ), key( k ), user( u ), lastSend_( -1e15 ) {}
        int id;
        std::string key;
        I_RosterQueryUser * user;
        std::vector< SAnswer > answers_;
        double lastSend_;
      };
      std::map< int, SQuery > queries_;
      int nextQuery_;

      struct Key {
        Key() {}
        Key( int i, char const * k ) : id( i ), key( k ), ack( false ) {}
        int id;
        std::string key;
        double lastSend;
        bool ack;
      };
      std::map< int, Key > keys_;
      std::map< double, Key * > keyQueue_;

      struct Peer {
        Peer() {}
        Peer( sockaddr_in const & s ) : sin_( s ), gotPeer_( false ), peer_( 0 ) {}
        sort_sockaddr_in sin_;
        bool gotPeer_;
        I_SocketPeer * peer_;
      };
      std::map< sort_sockaddr_in, Peer > peers_;
      std::map< double, Peer * > peerQueue_;
  };
}

I_RosterQuery * libintroduce::CreateRosterQuery()
{
  return static_cast< I_RosterQuery * >( new C_RosterQuery() );
}



C_RosterQuery::C_RosterQuery()
{
  socket_ = 0;
  serverPeer_ = 0;
  notify_ = 0;
  connected_ = false;
  lastConnectTime_ = 0;
  nextQuery_ = 0;
  lastRosterMessage_ = 0;
  lastTime_ = 0;
}

C_RosterQuery::~C_RosterQuery()
{
  assert( !serverPeer_ );
  assert( !notify_ );
}

namespace {
  class StopQuery {
    public:
      I_RosterQuery * query_;
      StopQuery( I_RosterQuery * q ) : query_( q ) {}
      void operator()( std::pair< int const, C_RosterQuery::SQuery > & q ) {
        q.second.user->onQueryTimeout( query_ );
      }
  };
}

void C_RosterQuery::disconnectSync()
{
  debug( "C_RosterQuery::disconnectSync() 0x%lx for %s\n", this, server_.staticText() );

  //  be kind and say good-bye
  ProtoHeader ph;
  ph.type = Disconnect;
  ph.size = 0;
  socket_->sendMessage( server_, &ph, sizeof( ph ) );
  std::for_each( queries_.begin(), queries_.end(), StopQuery( this ) );
  queries_.clear();
  keys_.clear();
  socket_ = 0;
  connected_ = false;
  lastRosterMessage_ = 0;
}

void C_RosterQuery::dispose()
{
  if( connected_ ) {
    disconnectSync();
  }
  if( notify_ ) {
    notify_->disposing( this );
    notify_ = 0;
  }
  if( serverPeer_ ) {
    serverPeer_->dispose();
    serverPeer_ = 0;
  }
  delete this;
}

void C_RosterQuery::connect( I_Socket * socket, char const * server, 
    char const * name, char const * password )
{
  char err[ 100 ];
  I_SocketPeer * peer = socket->createPeer( server, this, err, 100 );
  connectPeer( socket, peer, name, password );
}

void C_RosterQuery::connect( I_Socket * socket, sockaddr_in const & server, 
    char const * name, char const * password )
{
  I_SocketPeer * peer = socket->createPeer( server, this );
  connectPeer( socket, peer, name, password );
}

void C_RosterQuery::connectPeer( I_Socket * socket, I_SocketPeer * peer, 
  char const * name, char const * password )
{
  //  You really shouldn't re-use the same query for a different 
  //  server.
  assert( !connected_ );
  if( connected_ ) {
    disconnectSync();
  }
  assert( socket_ == 0 );
  socket_ = socket;
  serverPeer_ = peer;
  server_ = peer->peerAddress();
  userName_ = name;
  userPassword_ = password;
}

void C_RosterQuery::notify( I_RosterConnectionNotify * notify )
{
  notify_ = notify;
}

bool C_RosterQuery::connected()
{
  return connected_;
}

void C_RosterQuery::step( double curTime )
{
  assert( notify_ != NULL );
  maybeTimeoutRoster( curTime );
  maybeSendConnect( curTime );
  maybeSendQueries( curTime );
  maybeSendKeys( curTime );
  maybeSendIntroductions( curTime );
}

void C_RosterQuery::maybeTimeoutRoster( double curTime )
{
  lastTime_ = curTime;
  if( lastRosterMessage_ == 0 ) {
    lastRosterMessage_ = curTime;
  }
  if( lastTime_ > PEER_ROSTER_TIMEOUT + lastRosterMessage_ ) {
    debug( "C_RosterQuery::maybeTimeoutRoster() 0x%lx lastTime_ %g lastRosterMessage_ %g PEER_ROSTER_TIMEOUT %g\n",
        this, lastTime_, lastRosterMessage_, (double)PEER_ROSTER_TIMEOUT );
    notify_->onRosterTimeout( this, socket_ );
    lastRosterMessage_ = lastTime_;
  }
}

void C_RosterQuery::maybeSendQueries( double time )
{
  for( std::map< int, SQuery >::iterator ptr = queries_.begin();
      ptr != queries_.end();
      ++ptr ) {
    if( (*ptr).second.lastSend_ + QUERY_RESEND_SECONDS < time ) {

      debug( "C_RosterQuery::maybeSendQueries() 0x%lx time %g id %d\n", 
          this, time, (*ptr).second.id );

      (*ptr).second.lastSend_ = time;
      Message m;
      m.put( (*ptr).second.id );
      m.put( (*ptr).second.key.c_str() );
      ProtoHeader::prepend( Query, m );
      serverPeer_->sendMessage( m.buf_, m.size_ );
    }
  }
}

void C_RosterQuery::query( char const * key, I_RosterQueryUser * user, 
    int maxRecords )
{
  if( !serverPeer_ || !connected_ ) {
    debug( "C_RosterQuery::query() 0x%lx serverPeer_ 0x%lx connected_ %s\n",
        this, serverPeer_, connected_ ? "true" : "false" );
    user->onQueryTimeout( this );
    return;
  }
  int id = ++nextQuery_;
  SQuery q( id, key, user );
  queries_[id] = q;
}

void C_RosterQuery::maybeSendKeys( double curTime )
{
  if( !connected_ ) {
    return;
  }
  for( std::map<double, Key *>::iterator ptr = keyQueue_.begin();
      ptr != keyQueue_.end(); ) {
    std::map<double, Key * >::iterator i = ptr++;
    if( (*i).second->ack ) {
      keyQueue_.erase( i );
    }
    else if( curTime > (*i).first + KEY_RESEND_SECONDS ) {

      debug( "C_RosterQuery::maybeSendKeys() 0x%lx time %g sending key %d\n", this, curTime, (*i).second->id );
      
      Message m;
      m.put( (*i).second->id );
      m.put( (*i).second->key.c_str() );
      ProtoHeader::prepend( Record, m );
      serverPeer_->sendMessage( m.buf_, m.size_ );
      keyQueue_[curTime] = (*i).second;
      keyQueue_.erase( i );
    }
  }
}

void C_RosterQuery::add( char const * key, int recId )
{
  keys_[recId] = C_RosterQuery::Key( recId, key );
  keyQueue_[-1e15] = &keys_[recId];
}

void C_RosterQuery::maybeSendIntroductions( double curTime )
{
  if( !connected_ ) {
    return;
  }
  for( std::map<double, Peer *>::iterator ptr = peerQueue_.begin();
      ptr != peerQueue_.end(); ) {
    std::map< double, Peer * >::iterator i = ptr++;
    if( (*i).second->gotPeer_ ) {
      peerQueue_.erase( i );
    }
    else if( curTime > (*i).first + INTRODUCTION_RESEND_SECONDS ) {

      debug( "C_RosterQuery::maybeSendIntroductions() 0x%lx time %g sending to %s\n", 
          this, curTime, (*i).second->sin_.staticText() );

      Message m;
      m.put( (*i).second->sin_ );
      ProtoHeader::prepend( IntroducePeer, m );
      serverPeer_->sendMessage( m.buf_, m.size_ );
      peerQueue_[curTime] = (*i).second;
      peerQueue_.erase( i );
    }
  }
}

//  The "peer" here is still the server.
void C_RosterQuery::onIntroduceAck( I_SocketPeer * peer, Message & m )
{
  debug( "C_RosterQuery::onIntroduceAck() 0x%lx peer 0x%lx %d bytes\n", this, peer, m.size_ );

  sort_sockaddr_in sin;
  if( !m.get( sin ) ) {
    //  fixme: don't use assert to report errors
    assert( !"bad data in onIntroduceAck" );
    return;
  }
  //  fixme: perhaps the login name, as seen by roster, should go with this 
  //  inet address, so that we can know the roster validated the name.
  std::map< sort_sockaddr_in, Peer >::iterator ptr = peers_.find( sin );
  Peer * p;
  if( ptr == peers_.end() ) {
    peers_[sin] = Peer( sin );
    p = &peers_[sin];
  }
  else {
    p = &(*ptr).second;
    bool found = false;
    for( std::map< double, Peer * >::iterator i = peerQueue_.begin(); 
        i != peerQueue_.end(); ++i ) {
      if( (*i).second == p ) {
        peerQueue_.erase( i );
        found = true;
        break;
      }
    }
    assert( found );
  }
  p->gotPeer_ = true;
  p->peer_ = socket_->createPeer( sin, 0 );
  notify_->onPeerConnection( this, socket_, p->peer_ );
}

void C_RosterQuery::introduce( sockaddr_in const & s )
{
  sort_sockaddr_in ssin( s );
  debug( "C_RosterQuery::introduce() 0x%lx addr %s\n", this, ssin.staticText() );

  //  you must configure a notify_ before calling introduce()
  assert( notify_ );
  if( !notify_ ) {
    warning( "you must configure a notify() before calling C_RosterQuery::introduce()" );
  }
  sort_sockaddr_in sin = s;
  std::map< sort_sockaddr_in, Peer >::iterator ptr = peers_.find( sin );
  if( ptr != peers_.end() ) {
    return; //  already working on it
  }
  peers_[sin] = Peer( s );
  peerQueue_[1e-15] = &peers_[sin];
}

void C_RosterQuery::disposing( I_Socket * socket )
{
  assert( !"not implemented" );
  connected_ = false;
}

void C_RosterQuery::onConnection( I_Socket * socket, sockaddr_in const & addr, 
    char const * buf, int bSize )
{
  assert( !"not implemented" );
}

void C_RosterQuery::disposing( I_SocketPeer * peer )
{
  connected_ = false;
}

void C_RosterQuery::onMessage( I_SocketPeer * peer, char const * buf, int bSize )
{
  if( peer == serverPeer_ ) {
    debug( "C_RosterQuery::onMessage() 0x%lx lastTime_ %g size %d type %d\n", 
        this, lastTime_, bSize, buf[0] );
    lastRosterMessage_ = lastTime_;

    Message m( buf, bSize );
    ProtoHeader ph;
    if( !m.getn( sizeof( ph ), &ph ) ) {
      return;
    }
    switch( ph.type ) {
    case ConnectAck:
      connected_ = true;
      break;
    case Answer:
      onAnswer( peer, m );
      break;
    case Kick:
      assert( !"implement me" );
      break;
    case RecordAck:
      onRecordAck( peer, m );
      break;
    case IntroduceAck:
      onIntroduceAck( peer, m );
      break;
    case Keepalive:
      //  nothing
      debug( "C_RosterQuery::onMessage() 0x%lx keepalive\n", this );
      break;
    default:
      warning( "C_RosterQuery::onMessage() 0x%lx: unknown reply type '%d' received\n", 
          this, ph.type );
      //fixme: don't use assert for error reporting
      assert( !"unknown reply type" );
      break;
    }
  }
  else {
      warning( "C_RosterQuery::onMessage() 0x%lx: unexpected message from unknown, existing peer (%s)\n", 
          this, sort_sockaddr_in( peer->peerAddress() ).staticText() );
    //fixme: do I need to vector through to the user here?
    assert( !"unexpected message from unknown, but existing, peer" );
  }
}

void C_RosterQuery::onRecordAck( I_SocketPeer * peer, Message & m )
{
  debug( "C_RosterQuery::onRecordAck() 0x%lx peer 0x%lx %d bytes\n", this, peer, m.size_ );

  int id;
  m.get( &id );
  std::map< int, Key >::iterator ptr = keys_.find( id );
  if( ptr == keys_.end() ) {
    //  this was an ID that's not actually ever defined
    assert( !"bad ID received" );
    return;
  }
  (*ptr).second.ack = true;
}

void C_RosterQuery::onAnswer( I_SocketPeer * peer, Message & m )
{
  int id;
  int num;
  int ix;
  float score;
  char buf[ 1024 ];
  sockaddr_in addr;
  std::map< int, SQuery >::iterator ptr;

  debug( "C_RosterQuery::onAnswer() 0x%lx peer 0x%lx %d bytes\n", this, peer, m.size_ );

  if( !m.get( &id ) ) { assert( 0 ); goto error; }
  if( !m.get( &num ) ) { assert( 0 ); goto error; }
  if( !m.get( &ix ) ) { assert( 0 ); goto error; }
  if( !m.get( &score ) ) { assert( 0 ); goto error; }
  if( !m.get( buf, sizeof( buf ) ) ) { assert( 0 ); goto error; }
  if( !m.getn( sizeof( addr ), &addr ) ) { assert( 0 ); goto error; }

  //  sanity check
  if( num == 0 ) {
    assert( ix == 0 );
    debug( "C_RosterQuery::onAnswer() 0x%lx received empty answer\n", this );
    num = 1;
  }
  if( ix < 0 || ix >= num ) { assert( 0 ); goto error; }
  if( num < 0 || num > MAX_MATCHED_RECORDS ) { assert( 0 ); goto error; }

  //  find the corresponding query
  ptr = queries_.find( id );
  if( ptr == queries_.end() ) { assert( 0 ); goto error; }
  if( (*ptr).second.answers_.size() != size_t(num) ) {
    (*ptr).second.answers_.resize( size_t(num) );
  }
  {
    SAnswer & a = (*ptr).second.answers_[ ix ];
    a.ok_ = true;
    a.data_ = buf;
    a.addr_ = addr;
    a.score_ = score;
  }
  maybeNotifyUser( id, (*ptr).second );

  return;

error:
  ; // do nothing
}

void C_RosterQuery::maybeNotifyUser( int id, SQuery & q )
{
  debug( "C_RosterQuery::maybeNotifyUser() 0x%lx id %d\n", this, id );

  for( std::vector< SAnswer >::iterator ptr = q.answers_.begin();
      ptr != q.answers_.end();
      ++ptr ) {
    if( !(*ptr).ok_ ) {
      return; //  not done -- don't notify user
    }
  }
  //  I guess we're done! Create the MatchedRecord array
  std::vector< MatchedRecord > mrv;
  for( std::vector< SAnswer >::iterator ptr = q.answers_.begin();
      ptr != q.answers_.end();
      ++ptr ) {
    MatchedRecord mr;
    mr.addr = (*ptr).addr_;
    mr.record = (*ptr).data_.c_str();
    mr.score = (*ptr).score_;
    mrv.push_back( mr );
  }
  q.user->onQueryResult( this, &mrv[0], int( mrv.size() ) );
}

void C_RosterQuery::maybeSendConnect( double time )
{
  if( serverPeer_ && (time >= CONNECT_RETRY_SECONDS + lastConnectTime_) ) {
    if( !connected_ ) {
      debug( "C_RosterQuery::maybeSendConnect() 0x%lx time %g lastConnectTime_ %g sending connect message\n", this, time, lastConnectTime_ );
      lastConnectTime_ = time;
      Message m;
      m.put( userName_.c_str() );
      m.put( userPassword_.c_str() );
      ProtoHeader::prepend( Connect, m );
      serverPeer_->sendMessage( m.buf_, m.size_ );
    }
    else if( time > (PEER_TIMEOUT_SECONDS / 5 + lastConnectTime_) ) {
      debug( "C_RosterQuery::maybeSendConnect() 0x%lx time %g lastConnectTime_ %g sending keepalive\n", this, time, lastConnectTime_ );
      lastConnectTime_ = time;
      ProtoHeader ph;
      ph.type = Keepalive;
      ph.size = 0;
      serverPeer_->sendMessage( &ph, sizeof( ph ) );
    }
  }
}


namespace {
  class C_RosterSocketUser : public I_SocketUser {
    public:
      C_RosterSocketUser() {}
      void disposing( I_Socket * socket );
      void onConnection( I_Socket * socket, sockaddr_in const & addr, 
          char const * buf, int bSize );
  };
}

void C_RosterSocketUser::disposing( I_Socket * socket )
{
  warning( "C_RosterSocketUser::disposing() 0x%lx shouldn't be here\n", this );
  assert( !"not implemented" );
}

void C_RosterSocketUser::onConnection( I_Socket * socket, sockaddr_in const & addr, 
    char const * buf, int bSize )
{
  warning( "C_RosterSocketUser::onConnection() 0x%lx shouldn't be here\n", this );
  assert( !"not implemented" );
}

namespace {
  class C_RosterQueryUser : public I_RosterQueryUser {
    public:
      C_RosterQueryUser( std::list< std::string > & strings ) :
        strings_( strings ), done_( false ), numMatches_( -1 ) {}
      void disposing( I_RosterQuery * query ) {}
      void onQueryResult( I_RosterQuery * query, 
          MatchedRecord const * matched, int numMatched );
      void onQueryTimeout( I_RosterQuery * query );

      std::vector< MatchedRecord > matches_;
      std::list< std::string > & strings_;
      bool done_;
      int numMatches_;
  };
}

void C_RosterQueryUser::onQueryResult( I_RosterQuery * query, 
    MatchedRecord const * matched, int numMatched )
{
  debug( "C_RosterQueryUser::onQueryResult() 0x%lx with %d matches\n", this, numMatched );

  matches_.resize( numMatched );
  memcpy( &matches_[0], matched, sizeof(MatchedRecord)*numMatched );
  strings_.clear();
  for( size_t ix = 0; ix < size_t( numMatched ); ++ix ) {
    strings_.push_back( matched[ix].record );
    matches_[ix].record = strings_.back().c_str();
  }
  done_ = true;
  numMatches_ = numMatched;
}

void C_RosterQueryUser::onQueryTimeout( I_RosterQuery * query )
{
  debug( "C_RosterQueryUser::onQueryTimeout() 0x%lx\n", this );

  done_ = true;
}

int libintroduce::SimpleRosterQuery( I_Network * nw, 
    char const * server, 
    char const * user, char const * password, 
    char const * key, 
    MatchedRecord * outRecords, int maxRecords )
{
  //  space for returned strings
  static std::list< std::string > strings;
  strings.swap( std::list< std::string >() );

  //  create local socket
  C_RosterSocketUser rsu;
  I_Socket * s = 0;
  for( int i = 0; i < 10; ++i ) {
    s = nw->createSocket( 45234+i, &rsu );
    if( s ) {
      break;
    }
  }
  if( !s ) {
    warning( "could not bind to any port 45234 through 45243" );
    return -1;
  }

  //  create our query object
  I_RosterQuery * q = CreateRosterQuery();

  //  connect
  q->connect( s, server, user, password );

  C_RosterQueryUser rqu( strings );
  I_Clock * c = CreateClock();
  double now = c->realTime();
  int result = -1;

  while( !q->connected() ) {
    double then = c->realTime();
    if( then > now + SIMPLE_TIMEOUT_SECONDS ) {
      warning( "timeout waiting to connect to server" );
      goto error;
    }
    q->step( then );
    nw->step( 0 );
  }

  q->query( key, &rqu, 10 );
  now = c->realTime();
  double cur;
  while( !rqu.done_ && ((cur = c->realTime()) < (now + SIMPLE_TIMEOUT_SECONDS)) ) {
    q->step( cur );
    nw->step( 0.25f );
  }
  result = rqu.numMatches_;
  if( result > maxRecords ) {
    result = maxRecords;
  }
  if( result > 0 ) {
    for( int ix = 0; ix < result; ++ix ) {
      outRecords[ix] = rqu.matches_[ix];
    }
  }

error:
  c->dispose();
  q->dispose();
  s->dispose();

  return result;
}


#include "test_fakes.h"

namespace libintroduce {
extern int swallowWarnings;
}


void Unittest_Roster()
{
  TestFakeSock fakeSock;
  TestRosterUser tru;
  TestFakeClock clock;
  char str[ 512 ];

  //  verify lifetime with provided socket
  I_Roster * r = CreateRoster( (I_Clock *)&clock, (I_Socket *)&fakeSock, &tru );
  assert( r->network() == (I_Network *)&fakeSock );
  assert( r->socket() == (I_Socket *)&fakeSock );
  assert( r->rosterUser() == (I_RosterUser *)&tru );
  Message msg;
  msg.put( "root" );
  msg.put( "password" );
  ProtoHeader::prepend( Connect, msg );
  fakeSock.sockUser_->onConnection( &fakeSock, fakeSock.peer_.peerAddress_, msg.buf_, msg.size_ );
  r->step();
  assert( fakeSock.gotCreatePeer_ ); fakeSock.gotCreatePeer_ = false;
  r->addRecord( fakeSock.peer_.peerAddress_, "name=blah;size=3;time=now", 1 );
  r->addRecord( fakeSock.peer_.peerAddress_, "blarg=0", 2 );
  //  make sure we replace correctly
  r->addRecord( fakeSock.peer_.peerAddress_, "name=booh;size=2;time=then", 2 );
  r->step();
  MatchedRecord matches[ 10 ];
  int m = r->matchRecords( "name=blah", matches, 10, 1.0f );
  assert( m == 1 );
  m = r->matchRecords( "name=b%;size<3", matches, 10, 0.5f );
  assert( m == 2 );
  m = r->matchRecords( "name=b%;size<3", matches, 1, 0.5f );
  assert( m == 1 );
  m = r->matchRecords( "name=b%;size<3", matches, 10, 1.0f );
  assert( m == 1 );
  m = r->matchRecords( "name=c%;size>3", matches, 10, 0.1f );
  assert( m == 0 );

  r->removeRecords( fakeSock.peer_.peerAddress_ );
  r->dispose();
  assert( tru.gotDisposing_ ); tru.gotDisposing_ = false;
  assert( !fakeSock.gotDispose_ );

  //  verify lifetime with network-created socket
  r = CreateRoster( (I_Clock *)&clock, (I_Network *)&fakeSock, 12345, &tru );
  assert( fakeSock.gotPort_ == 12345 );
  C_Roster * cr = static_cast< C_Roster * >( r );

  //  getting a message from someone who's not logged in should kick them
  Message mm;
  ProtoHeader::prepend( Keepalive, mm );
  ++swallowWarnings;
  fakeSock.sockUser_->onConnection( &fakeSock, fakeSock.peer_.peerAddress_, mm.buf_, mm.size_ );
  assert( ((ProtoHeader *)fakeSock.peer_.message_)->type == Kick );

  //  connection with invalid name/password should not work
  mm.clear();
  mm.put( "bad" );
  mm.put( "bad" );
  ProtoHeader::prepend( Connect, mm );
  ((ProtoHeader *)fakeSock.peer_.message_)->type = 0x7f;
  fakeSock.sockUser_->onConnection( &fakeSock, fakeSock.peer_.peerAddress_, mm.buf_, mm.size_ );
  assert( ((ProtoHeader *)fakeSock.peer_.message_)->type == 0x7f );

  //  connection with valid name/password should work
  mm.clear();
  mm.put( "root" );
  mm.put( "password" );
  ProtoHeader::prepend( Connect, mm );
  fakeSock.sockUser_->onConnection( &fakeSock, fakeSock.peer_.peerAddress_, mm.buf_, mm.size_ );
  assert( ((ProtoHeader *)fakeSock.peer_.message_)->type == ConnectAck );

  //  if we get a second connect request, it should be re-acked
  ((ProtoHeader *)fakeSock.peer_.message_)->type = 0x7f;
  fakeSock.peer_.user_->onMessage( &fakeSock.peer_, mm.buf_, mm.size_ );
  assert( ((ProtoHeader *)fakeSock.peer_.message_)->type == ConnectAck );

  r->step();

  //  keepalives should not generate response traffic
  mm.clear();
  ProtoHeader::prepend( Keepalive, mm );
  ((ProtoHeader *)fakeSock.peer_.message_)->type = 0x7f;
  fakeSock.peer_.user_->onMessage( &fakeSock.peer_, mm.buf_, mm.size_ );
  assert( ((ProtoHeader *)fakeSock.peer_.message_)->type == 0x7f);

  //  register a record
  mm.clear();
  mm.put( 1 );
  mm.put( "record=1;option=true" );
  ProtoHeader::prepend( Record, mm );
  fakeSock.peer_.user_->onMessage( &fakeSock.peer_, mm.buf_, mm.size_ );
  mm = Message( fakeSock.peer_.message_, fakeSock.peer_.messageSize_ );
  ProtoHeader ph;
  assert( mm.getn( sizeof( ph ), &ph ) );
  assert( ph.type == RecordAck );
  assert( ph.size == sizeof( int ) );
  int i;
  float f;
  assert( mm.get( &i ) );
  assert( i == 1 );

  //  run a query
  mm.clear();
  mm.put( 10 );             //  query identifier
  mm.put( "option=true" );  //  query string
  ProtoHeader::prepend( Query, mm );
  fakeSock.peer_.user_->onMessage( &fakeSock.peer_, mm.buf_, mm.size_ );
  mm = Message( fakeSock.peer_.message_, fakeSock.peer_.messageSize_ );
  assert( mm.getn( sizeof( ph ), &ph ) );
  assert( ph.type == Answer );
  assert( mm.get( &i ) );
  assert( i == 10 );
  assert( mm.get( &i ) );
  assert( i == 1 );
  assert( mm.get( &i ) );
  assert( i == 0 );
  assert( mm.get( &f ) );
  assert( f == 1.0 );
  assert( mm.get( str, sizeof( str ) ) );
  assert( !strcmp( str, "record=1;option=true" ) );
  sort_sockaddr_in sin;
  assert( mm.getn( sizeof( sin ), &sin ) );
  assert( sin == fakeSock.peer_.peerAddress_ );
  assert( mm.size_ == mm.base_ );

  //  shouldn't be kicked the first time (still within timeout)
  clock.time_ += C_Roster::peerTimeout_/2 + 0.125f;
  ((ProtoHeader *)fakeSock.peer_.message_)->type = 0x7f;
  r->step();
  assert( ((ProtoHeader *)fakeSock.peer_.message_)->type == 0x7f);

  //  should be kicked the second time (now outside timeout)
  clock.time_ += C_Roster::peerTimeout_/2 + 0.125f;
  r->step();
  assert( ((ProtoHeader *)fakeSock.peer_.message_)->type == Kick);
  assert( cr->peers_.size() == 0 );
  assert( cr->records_.size() == 0 );

  r->dispose();
  assert( tru.gotDisposing_ ); tru.gotDisposing_ = false;
  assert( fakeSock.gotDispose_ ); fakeSock.gotDispose_ = false;
}


void Unittest_RosterQuery()
{
  //  test querying
  TestNetwork tnw;
  MatchedRecord matches[ 10 ];
  Message m;
  //  add a reply for a possible Connect packet
  ProtoHeader ph;
  ph.type = ConnectAck;
  ph.size = 0;
  m.putn( sizeof( ph ), &ph );
  tnw.addReply( Connect, tnw.tfs_.peer_.peerAddress_, m );
  //  add an Answer reply for a possible Query packet
  m.clear();
  m.put( 1 );
  m.put( 1 );
  m.put( 0 );
  m.put( 1.0f );
  m.put( "value=1" );
  sort_sockaddr_in sin;
  memset( &sin, 1, sizeof( sin ) );
  m.putn( sizeof( sin ), &sin );
  ProtoHeader::prepend( Answer, m );
  tnw.addReply( Query, tnw.tfs_.peer_.peerAddress_, m );
  int i = SimpleRosterQuery( &tnw, "host1:45234", "root", "password", "value=1", matches, 10 );
  assert( i == 1 );
  assert( sin == matches[0].addr );

  //  test adding records
  TestFakeClock tfc;
  I_Clock * c = &tfc;
  I_RosterQuery * rq = CreateRosterQuery();
  I_Socket * sock = tnw.createSocket( 12345, 0 );
  rq->connect( sock, "server:44444", "root", "password" );
  int cnt = 0;
  while( !rq->connected() ) {
    assert( ++cnt < 3 );    //  I should connect immediately
    rq->step( c->realTime() );
    tnw.step( 0.01 );
  }
  //  set up fake response for Record query
  m.clear();
  m.put( 42 );
  ProtoHeader::prepend( RecordAck, m );
  tnw.addReply( Record, tnw.tfs_.peer_.peerAddress_, m );

  rq->add( "me=this;value=5", 42 );
  //  Send, and ack, are the two events we expect. Give 
  //  then a step each (although stepping network after rq 
  //  will mean it's probably done in the first step).
  for( int ix = 0; ix < 2; ++ix ) {
    rq->step( c->realTime() );
    tnw.step( 0.01 );
  }
  C_RosterQuery * crq = static_cast< C_RosterQuery * >( rq );
  assert( (crq->keys_.size() == 1) && (*crq->keys_.begin()).second.ack );

  //  test connecting to other peers
  SIN_ADDR_UINT(sin.sin_addr) = 0x03030303;
  sin.sin_port = htons( 44444 );
  m.clear();
  m.put( sin );
  ProtoHeader::prepend( IntroduceAck, m );
  tnw.addReply( IntroducePeer, tnw.tfs_.peer_.peerAddress_, m );
  TestRosterConnectionNotify trcn;
  rq->notify( &trcn );
  rq->introduce( sin );
  for( int ix = 0; ix < 2; ++ix ) {
    rq->step( c->realTime() );
    tnw.step( 0.01 );
  }
  assert( trcn.peer_ );

  rq->dispose();
  c->dispose();
}

