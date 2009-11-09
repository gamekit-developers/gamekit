
#include "sock_port.h"
#include "network.h"
#include "sort_addr.h"

#include <vector>
#include <map>

#include <assert.h>


using namespace libintroduce;

namespace {
  class C_Socket;
  class C_SocketPeer;
}

static void (*Warn_Func)( int level, char const *, va_list );

void libintroduce::warning( char const * fmt, ... )
{
  va_list vl;
  va_start( vl, fmt );
  (*Warn_Func)( WL_WARNING, fmt, vl );
  va_end( vl );
}

void libintroduce::log( char const * fmt, ... )
{
  va_list vl;
  va_start( vl, fmt );
  (*Warn_Func)( WL_LOG, fmt, vl );
  va_end( vl );
}

void libintroduce::debug( char const * fmt, ... )
{
  va_list vl;
  va_start( vl, fmt );
  (*Warn_Func)( WL_DEBUG, fmt, vl );
  va_end( vl );
}

namespace {
  class C_Network : public I_Network {
  public:
    C_Network();
    ~C_Network();
    
    //  I_Network
    void dispose();
    I_Socket * createSocket( unsigned short port, I_SocketUser * user );
    void step( double sleepSeconds );

    void remove( C_Socket * socket );

    void reallocate( size_t i );

    std::vector< pollfd > pollFds_;
    std::vector< C_Socket * > sockets_;
  };
}

namespace {
  class C_Socket : public I_Socket {
  public:
    C_Socket( C_Network * network );
    ~C_Socket();
    
    C_Network * network_;
    I_SocketUser * user_;
    SOCKET fd_;
    short port_;

    bool configure( short port, I_SocketUser * user );
    void remove( C_SocketPeer * peer );
    void receive( sockaddr_in const & addr, char const * buf, int size );

    void dispose();
    I_Network * network();
    I_SocketUser * socketUser();
    void setSocketUser( I_SocketUser * user );
    I_SocketPeer * createPeer( sockaddr_in const & addr, I_SocketPeerUser * user );
    I_SocketPeer * createPeer( char const * addr, I_SocketPeerUser * user, 
        char * oError, int eSize );
    I_SocketPeer * findPeer( sockaddr_in const & addr );
    void sendMessage( sockaddr_in const & addr, void const * buf, int bSize );
    sockaddr_in address();
    void reallocate();

    typedef std::map< sort_sockaddr_in, C_SocketPeer * > PeerMap;
    PeerMap peers_;
  };
}


namespace {
  class C_SocketPeer : public I_SocketPeer {
  public:
    C_SocketPeer( C_Socket * socket );
    ~C_SocketPeer();
    
    C_Socket * socket_;
    I_SocketPeerUser * user_;
    sort_sockaddr_in sin_;

    void configure( sort_sockaddr_in const & sin, I_SocketPeerUser * user );

    void dispose();
    sockaddr_in const & peerAddress();
    I_Socket * socket();
    I_SocketPeerUser * socketPeerUser();
    void sendMessage( void const * buf, int bSize );
  };
}



I_Network * libintroduce::CreateNetwork( char * buf, int size, void (*warnFunc)( int level, char const *, va_list ) )
{
  static bool socketLibraryInited;
  if( !socketLibraryInited ) {
    INIT_SOCKET_LIBRARY();
    ENABLE_FIREWALL();
  }
  Warn_Func = warnFunc;
  C_Network * nw = new C_Network();
  return static_cast< I_Network * >( nw );
}


C_Network::C_Network()
{
}

C_Network::~C_Network()
{
  assert( !sockets_.size() );
  assert( !pollFds_.size() );
}

void C_Network::dispose()
{
  size_t s;
  while( (s = sockets_.size()) != 0 ) {
    libintroduce::warning( "I_Socket(%d:%d) not destroyed before C_Network::dispose()", 
        sockets_[sockets_.size()-1]->port_, sockets_[sockets_.size()-1]->fd_ );
    sockets_[sockets_.size()-1]->dispose();
    assert( s > sockets_.size() );
  }
  assert( sockets_.size() == pollFds_.size() );
  delete this;
}

I_Socket * C_Network::createSocket( unsigned short port, I_SocketUser * user )
{
  C_Socket * s = new C_Socket( this );
  if( !s->configure( port, user ) ) {
    libintroduce::warning( "C_Network::configure(%d) failed", port );
    s->dispose();
    return 0;
  }
  sockets_.push_back( s );
  pollfd pfd;
  pfd.revents = 0;
  pfd.events = POLLIN /*| POLLOUT*/;
  pfd.fd = s->fd_;
  pollFds_.push_back( pfd );
  assert( sockets_.size() == pollFds_.size() );
  return static_cast< I_Socket * >( s );
}

void C_Network::step( double sleepSeconds )
{
  char buffer[ MAX_MESSAGE_SIZE ];
  int size = sizeof( buffer );
  assert( sockets_.size() == pollFds_.size() );
  int r = ::poll( &pollFds_[0], pollFds_.size(), (int)(sleepSeconds*1000) );
  for( size_t i = 0; i < sockets_.size(); ++i ) {
    pollfd & pfd = pollFds_[i];
    if( pfd.revents & POLLIN ) {
      //  receive pending messages
      while( true ) {
        sockaddr_in addr;
        int len = sizeof( addr );
        size = ::recvfrom( pfd.fd, buffer, sizeof( buffer ), 
            NONBLOCK_MSG_SEND, (struct sockaddr *)&addr, &len );
        if( size < 1 ) {
          if( size < 0 ) {
            int e = SOCKET_ERRNO;
            if( SOCKET_WOULDBLOCK_ERROR( e ) ) {
              break;
            }
            else if( SOCKET_NEED_REOPEN( e ) ) {
              libintroduce::debug( "C_Network::step() 0x%lx: socket %d needs reallocate()\n", this, e );
              reallocate( i );
            }
          }
          break;
        }
        sockets_[i]->receive( addr, buffer, size );
      }
    }
    if( pfd.revents & POLLOUT ) {
      //  send queued messages
    }
    pfd.events = POLLIN /*| POLLOUT*/;
  }
}

void C_Network::remove( C_Socket * socket )
{
  size_t i = 0;
  while( i < sockets_.size() ) {
    if( sockets_[i] == socket ) {
      break;
    }
    ++i;
  }
  assert( i < sockets_.size() );
  sockets_.erase( &sockets_[i] );
  pollFds_.erase( &pollFds_[i] );
}

void C_Network::reallocate( size_t i )
{
  assert( i >= 0 && i < sockets_.size() );
  sockets_[i]->reallocate();
  pollFds_[i].fd = sockets_[i]->fd_;
}




C_Socket::C_Socket( C_Network * network )
{
  network_ = network;
  fd_ = BAD_SOCKET_FD;
  port_ = 0;
  user_ = 0;
}

C_Socket::~C_Socket()
{
  assert( fd_ == BAD_SOCKET_FD );
}

bool C_Socket::configure( short port, I_SocketUser * user )
{
  assert( port_ == 0 );
  assert( fd_ == BAD_SOCKET_FD );
  assert( port != 0 );

  //  if getprotoent() fails, we're screwed, and should core
  int udpProto = getprotobyname( "udp" )->p_proto;
  fd_ = ::socket( PF_INET, SOCK_DGRAM, udpProto );
  if( fd_ == BAD_SOCKET_FD ) {
    libintroduce::warning( "could not create local UDP socket" );
    return false;
  }

  int r = 0;
  MAKE_SOCKET_NONBLOCKING( fd_, r );
  if( r < 0 ) {
    libintroduce::warning( "could not make socket %d non-blocking", fd_ );
    ::closesocket( fd_ );
    fd_ = BAD_SOCKET_FD;
    return false;
  }

  int one = 1;
  r = ::setsockopt( fd_, SOL_SOCKET, SO_BROADCAST, (char const *)&one, sizeof(one) );

  sockaddr_in sin;
  memset( &sin, 0, sizeof( sin ) );
  sin.sin_family = AF_INET;
  sin.sin_port = htons( port );
  r = ::bind( fd_, (struct sockaddr *)&sin, sizeof( sin ) );
  if( r < 0 ) {
    libintroduce::warning( "could not bind socket %d to local port %d", fd_, port );
    ::closesocket( fd_ );
    fd_ = BAD_SOCKET_FD;
    return false;
  }
  port_ = port;
  user_ = user;

  libintroduce::debug( "C_Socket::configure() 0x%lx: socket %d port %d\n", this, fd_, port_ );

  return true;
}

void C_Socket::remove( C_SocketPeer * peer )
{
  sort_sockaddr_in sin = peer->peerAddress();
  PeerMap::iterator ptr = peers_.find( sin );
  assert( ptr != peers_.end() );
  if( ptr != peers_.end() ) {
    peers_.erase( ptr );
  }
}

void C_Socket::receive( sockaddr_in const & addr, char const * buf, int size )
{
  PeerMap::iterator ptr = peers_.find( addr );
  if( ptr == peers_.end() ) {
    user_->onConnection( this, addr, buf, size );
  }
  else {
    C_SocketPeer * peer = (*ptr).second;
    if( !peer->user_ ) {
      warning( "C_Socket::receive() 0x%lx for peer 0x%lx has NULL user; message size %d dropped\n",
          this, peer, size );
    }
    else {
      peer->user_->onMessage( peer, buf, size );
    }
  }
}

void C_Socket::dispose()
{
  assert( peers_.size() == 0 );
  if( user_ ) {
    user_->disposing( this );
    user_ = 0;
    //  user_ is only non-NULL if the configure was complete
    network_->remove( this );
  }
  network_ = 0;
  ::closesocket( fd_ );
  fd_ = BAD_SOCKET_FD;
  delete this;
}

I_Network * C_Socket::network()
{
  return network_;
}

I_SocketUser * C_Socket::socketUser()
{
  return user_;
}

void C_Socket::setSocketUser( I_SocketUser * user )
{
  user_ = user;
}

I_SocketPeer * C_Socket::createPeer( sockaddr_in const & addr, I_SocketPeerUser * user )
{
  //  check for common mistakes and cull them out early
  if( SIN_ADDR_UINT(addr.sin_addr) == 0 || addr.sin_port == 0 ) {
    libintroduce::warning( "illegal arguments to C_Socket::createPeer(): 0x%x:%d 0x%lx\n",
        htonl(SIN_ADDR_UINT(addr.sin_addr)), htons(addr.sin_port), user );
    return 0;
  }

  sort_sockaddr_in sin = addr; // slicing assignment

  //  find out whether there is already a peer for this address
  PeerMap::iterator ptr = peers_.find( sin );
  if( ptr != peers_.end() ) {
    //  there already exists a peer for this address
    return 0;
  }

  //  make a new peer and remember it
  C_SocketPeer * sp = new C_SocketPeer( this );
  peers_[ sin ] = sp;
  sp->configure( addr, user );
  return sp;
}

I_SocketPeer * C_Socket::createPeer( char const * addr, I_SocketPeerUser * user, 
    char * oError, int eSize )
{
  //  check for common argument problems
  if( !addr || !user || !oError || eSize < 10 ) {
    libintroduce::warning( "bad arguments to C_Socket::createPeer()\n" );
    return 0;
  }

  //  parse the port part of the host name (colon not allowed in host name)
  char const * colon = strchr( addr, ':' );
  //  default to the configured port if we don't get another value
  short port = port_;
  if( colon ) {
    char * end = 0;
    long l = strtol( colon+1, &end, 10 );
    if( l == 0 || l < 0 || l > 65535 ) {
      //  only allow legal port ids
      snprintf( oError, eSize, "createPeer() got bad port from address: '%s'", colon+1 );
      return 0;
    }
    if( !end || *end != 0 ) {
      //  don't allow sloppy port ids
      snprintf( oError, eSize, "createPeer() got bad port id format: '%s'", end ? end : "" );
      return 0;
    }
    port = short( l & 0xffff );
  }
  if( !colon ) {
    //  make sure we point at the end of the string for memcpy()
    colon = addr + strlen( addr );
  }
  //  extract the host name portion
  char hostname[ 256 ];
  if( colon-addr > 255 ) {
    snprintf( oError, eSize, "createPeer() got too long host name: %d chars", colon-addr );
    return 0;
  }
  memcpy( hostname, addr, colon-addr );
  hostname[ colon-addr ] = 0;
  if( !hostname[0] ) {
    snprintf( oError, eSize, "createPeer() got an empty host name" );
    return 0;
  }
  //  ask the deliciously blocking, non-thread-safe host resolver for the 
  //  address of this host.
  struct hostent * hent;
  hent = gethostbyname( hostname );
  if( !hent ) {
    snprintf( oError, eSize, "createPeer() could not find host: %s (%s)", hostname, hstrerror( h_errno ) );
    return 0;
  }

  //  formulate the actual address to use
  sockaddr_in sin;
  memset( &sin, 0, sizeof( sin ) );
  sin.sin_family = AF_INET;
  sin.sin_addr = *(in_addr *)hent->h_addr_list[0];
  sin.sin_port = htons( port );
  
  //  attempt to create a peer using the given address
  return createPeer( sin, user );
}

I_SocketPeer * C_Socket::findPeer( sockaddr_in const & addr )
{
  PeerMap::iterator ptr = peers_.find( addr );
  if( ptr != peers_.end() ) {
    //  found it!
    return (*ptr).second;
  }
  // not found
  return 0;
}

void C_Socket::sendMessage( sockaddr_in const & addr, void const * buf, int bSize )
{
  int r = ::sendto( fd_, reinterpret_cast< char const * >( buf ), bSize, 
      NONBLOCK_MSG_SEND, (struct sockaddr *)&addr, sizeof( addr ) );
  if( r < 0 ) {
    warning( "C_Socket::sendMessage() 0x%lx error sending to %s: %d\n", this, 
        sort_sockaddr_in( addr ).staticText(), SOCKET_ERRNO );
  }
}

sockaddr_in C_Socket::address()
{
  sockaddr_in sin;
  memset( &sin, 0, sizeof( sin ) );
  int sin_len = (int)sizeof( sin );
  ::getsockname( fd_, (struct sockaddr *)&sin, &sin_len );
  return sin;
}



void C_Socket::reallocate()
{
  int udpProto = getprotobyname( "udp" )->p_proto;
  SOCKET s2 = ::socket( PF_INET, SOCK_DGRAM, udpProto );
  if( s2 == BAD_SOCKET_FD ) {
    libintroduce::warning( "could not allocate a new socket to reallocate socket %d: error %d\n", fd_, SOCKET_ERRNO );
    return;
  }
  int r = 0;
  MAKE_SOCKET_NONBLOCKING( s2, r );
  if( r < 0 ) {
    warning( "could not make socket %d non-blocking", s2 );
    ::closesocket( s2 );
    return;
  }

  libintroduce::debug( "C_Socket::reallocate() 0x%lx: socket %d becomes %d for port %d\n", this, fd_, s2, port_ );

  ::closesocket( fd_ );
  fd_ = s2;

  sockaddr_in sin;
  memset( &sin, 0, sizeof( sin ) );
  sin.sin_family = AF_INET;
  sin.sin_port = htons( port_ );
  r = ::bind( fd_, (struct sockaddr *)&sin, sizeof( sin ) );
  if( r < 0 ) {
    libintroduce::warning( "could not re-bind new socket %d to local port %d", fd_, port_ );
    ::closesocket( fd_ );
    fd_ = BAD_SOCKET_FD;
  }
}



C_SocketPeer::C_SocketPeer( C_Socket * socket )
{
  assert( socket );
  socket_ = socket;
  user_ = 0;
  memset( &sin_, 0, sizeof( sin_ ) );
}

C_SocketPeer::~C_SocketPeer()
{
  assert( !socket_ );
  assert( !user_ );
}

void C_SocketPeer::configure( sort_sockaddr_in const & sin, I_SocketPeerUser * user )
{
  assert( socket_ );
  assert( sin.sin_port );
  assert( SIN_ADDR_UINT(sin.sin_addr) );
  assert( !user_ );
  user_ = user;
  sin_ = sin;
}

void C_SocketPeer::dispose()
{
  if( user_ ) {
    user_->disposing( this );
  }
  socket_->remove( this );
  user_ = 0;
  socket_ = 0;
  delete this;
}

sockaddr_in const & C_SocketPeer::peerAddress()
{
  return sin_;
}

I_Socket * C_SocketPeer::socket()
{
  return socket_;
}

I_SocketPeerUser * C_SocketPeer::socketPeerUser()
{
  return user_;
}

void C_SocketPeer::sendMessage( void const * buf, int bSize )
{
  //  Easiest is to just send here, and let the kernel sort out the 
  //  buffering. If I flood the link, then this will block and/or fail 
  //  at some point.
  //  An alternative is to copy the data into a queue per-socket, and 
  //  use ::poll() to send only on sockets that are ready to accept 
  //  more data. Possibly, in a non-blocking fashion. However, the 
  //  queue still needs to be limited length, and in the end, it just 
  //  translates to possibly dropping data anyway.
  //  Note that I don't guarantee success of the message (there can be 
  //  no such guarantee) so you'll have to detect transmission failure 
  //  somehow anyway.
  int r = ::sendto( socket_->fd_, reinterpret_cast< char const * >( buf ), bSize, 
      NONBLOCK_MSG_SEND, (struct sockaddr *)&sin_, sizeof( sin_ ) );
  if( r < 0 ) {
    warning( "C_Socket::sendMessage() 0x%lx error sending to %s: %d\n", this, 
        sin_.staticText(), SOCKET_ERRNO );
  }
}


#include <stdio.h>  

namespace libintroduce {

int swallowWarnings = 0;

}

void Unittest_Warn_Func( int level, char const * fmt, va_list vl )
{
  if( level < WL_WARNING ) {
    return;
  }
  if( !swallowWarnings ) {
    fprintf( stderr, "WARNING: " );
    vfprintf( stderr, fmt, vl );
    fprintf( stderr, "\n" );
    assert( 0 );
  }
  else {
    --swallowWarnings;
  }
}

void Unittest_Network()
{
  char buf[ 100 ];
  I_Network * nw = libintroduce::CreateNetwork( buf, 100, Unittest_Warn_Func );
  assert( nw != 0 );
  nw->step(0.001);  //  sleeping
  nw->step(0);      //  polling
  nw->dispose();
}

namespace {
  //  This class is used as a convenient implementation of the 
  //  "user" interfaces, so that we can test the general system.
  class Unittest_SocketUser : public I_SocketUser, public I_SocketPeerUser {
  public:

    bool disposingCalled_;
    bool onConnectionCalled_;
    bool disposingPeerCalled_;
    bool onMessageCalled_;
    char buf_[ 128 ];
    int size_;
    sockaddr_in sin_;
    I_SocketPeer * peer_;

    Unittest_SocketUser() {
      disposingCalled_ = false;
      onConnectionCalled_ = false;
      disposingPeerCalled_ = false;
      onMessageCalled_ = false;
      size_ = 0;
      peer_ = 0;
    }
    void disposing( I_Socket * socket ) {
      disposingCalled_ = true;
    }
    void onConnection( I_Socket * socket, sockaddr_in const & addr, char const * buf, int bSize ) {
      onConnectionCalled_ = true;
      if( bSize > sizeof( buf_ ) ) {
        bSize = sizeof( buf_ );
      }
      memcpy( buf_, buf, bSize );
      size_ = bSize;
      sin_ = addr;
    }
    void disposing( I_SocketPeer * peer ) {
      disposingPeerCalled_ = true;
      peer_ = peer;
    }
    void onMessage( I_SocketPeer * peer, char const * buf, int bSize ) {
      onMessageCalled_ = true;
      if( bSize > sizeof( buf_ ) ) {
        bSize = sizeof( buf_ );
      }
      memcpy( buf_, buf, bSize );
      size_ = bSize;
      peer_ = peer;
    }
  };
}

void Unittest_Socket()
{
  char buf[ 100 ];
  I_Network * nw = libintroduce::CreateNetwork( buf, 100, Unittest_Warn_Func );
  assert( nw != 0 );

  //  verify that we can create and bind a socket on a port
  Unittest_SocketUser user1;
  I_Socket * sock1 = nw->createSocket( 60001, &user1 );
  assert( sock1->network() == nw );
  assert( sock1->socketUser() == &user1 );

  //  verify that we can't re-create a socket on the same port
  swallowWarnings = 2;
  assert( nw->createSocket( 60001, &user1 ) == 0 );

  //  verify that we don't have a peer (this just tests searching 
  //  when there are no peers).
  sockaddr_in sin;
  sin.sin_family = AF_INET;
  sin.sin_port = htons( 60001 );
  SIN_ADDR_UINT( sin.sin_addr ) = htonl( 0x7f0001 );
  assert( sock1->findPeer( sin ) == 0 );

  //  step for good measure
  nw->step( 0 );

  //  verify that we get properly notified on dispose
  assert( !user1.disposingCalled_ );
  sock1->dispose();
  assert( user1.disposingCalled_ );

  //  shut down the network
  nw->dispose();
}

static void Watch_And_Wait( I_Network * nw, bool volatile & flag )
{
  assert( flag == false );
  int watch = 50;
  while( watch > 0 && !flag ) {
    nw->step( 0.010 );
    --watch;
    assert( watch > 0 );  //  else it didn't happen
  }
  assert( flag );
}


void Unittest_SocketPeer()
{
  char buf[ 100 ];
  I_Network * nw = libintroduce::CreateNetwork( buf, 100, Unittest_Warn_Func );
  assert( nw != 0 );

  Unittest_SocketUser user1;
  I_Socket * sock1 = nw->createSocket( 60001, &user1 );
  Unittest_SocketUser user2;
  I_Socket * sock2 = nw->createSocket( 60002, &user2 );

  //  verify that we can create a peer
  sockaddr_in sin;
  sin.sin_family = AF_INET;
  sin.sin_port = htons( 60002 );
  SIN_ADDR_UINT( sin.sin_addr ) = htonl( 0x7f000001 );
  I_SocketPeer * peer1 = sock1->createPeer( sin, &user1 );
  assert( peer1 != 0 );

  //  verify that a message ends up inside "connection"
  peer1->sendMessage( "hello", 5 );
  Watch_And_Wait( nw, user2.onConnectionCalled_ );
  //  assert it's from who we think it is
  assert( user2.sin_.sin_port == htons( 60001 ) );

  //  verify that responding is reasonable
  assert( user1.peer_ == 0 );
  I_SocketPeer * peer2 = sock2->createPeer( user2.sin_, &user2 );
  assert( peer2 != 0 );
  peer2->sendMessage( "hello2", 6 );
  Watch_And_Wait( nw, user1.onMessageCalled_ );
  assert( user1.peer_ == peer1 );

  peer1->dispose();
  peer2->dispose();
  sock1->dispose();
  sock2->dispose();

  //  shut down the network
  nw->dispose();
}

