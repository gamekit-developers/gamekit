
#if !defined( test_fakes_h )
#define test_fakes_h

#include "network.h"
#include "message.h"

namespace libintroduce {

class TestFakeClock : public I_Clock {
  public:
    TestFakeClock() : time_( 1000 ) {}
    void dispose() {}
    void adjust( double delta ) { time_ += delta; }
    double realTime() { time_ += 0.01; return time_; }
    double time_;
};

class TestNetworkBase {
  public:
    TestNetworkBase() {}
    virtual void onMessageSent( char const * buf, size_t size ) = 0;
};

class TestFakeSockPeer : public I_SocketPeer {
  public:
    sockaddr_in peerAddress_;
    TestNetworkBase * nbase_;
    I_Socket * socket_;
    I_SocketPeerUser * user_;
    char message_[ 2048 ];
    int messageSize_;
    TestFakeSockPeer() : messageSize_( 0 ), nbase_( 0 ) {}
    virtual void dispose() { if( user_ ) user_->disposing( this ); }
    virtual sockaddr_in const & peerAddress() { return peerAddress_; }
    virtual I_Socket * socket() { return socket_; }
    virtual I_SocketPeerUser * socketPeerUser() { return user_; }
    virtual void sendMessage( void const * buf, int bSize ) { 
      assert( bSize < 2048 ); memcpy( message_, buf, bSize ); messageSize_ = bSize;
      if( nbase_ ) nbase_->onMessageSent( message_, messageSize_ ); }
};

class TestFakeSock : public I_Socket, public I_Network {
  public:
    TestFakeSock() : gotDispose_( false ), sockUser_( 0 ), gotCreatePeer_( false ) { peer_.socket_ = this; }

    // class I_Socket
    virtual void dispose() { gotDispose_ = true; if( sockUser_ ) sockUser_->disposing( this ); sockUser_ = 0; }
    virtual I_Network * network() { return this; }
    virtual I_SocketUser * socketUser() { return sockUser_; }
    virtual void setSocketUser( I_SocketUser * user ) { sockUser_ = user; }
    virtual I_SocketPeer * createPeer( sockaddr_in const & addr, I_SocketPeerUser * user ) {
        gotCreatePeer_ = true; peer_.peerAddress_ = addr; peer_.user_ = user; return &peer_; }
    virtual I_SocketPeer * createPeer( char const * addr, I_SocketPeerUser * user, 
        char * oError, int eSize ) { gotCreatePeer_ = true; memset( &peer_.peerAddress_, 2, sizeof( peer_.peerAddress_ ) ); 
                                      peer_.peerAddress_.sin_family = AF_INET; peer_.user_ = user; return &peer_; }
    virtual I_SocketPeer * findPeer( sockaddr_in const & addr ) { return &peer_; }
    virtual void sendMessage( sockaddr_in const & addr, void const * buf, int bSize ) {
        peer_.sendMessage( buf, bSize ); }
    virtual sockaddr_in address() {
        sockaddr_in sin; memset( &sin, 0, sizeof( sin ) ); sin.sin_family = AF_INET; sin.sin_port = htons(gotPort_);
        SIN_ADDR_UINT(sin.sin_addr) = 0x7f000001; return sin; }
    //  class I_Network
    virtual I_Socket * createSocket( unsigned short port, I_SocketUser * user ) { gotPort_ = port; sockUser_ = user; return this; }
    virtual void step( double sleepSeconds ) {}

    bool gotDispose_;
    bool gotCreatePeer_;
    short gotPort_;
    I_SocketUser * sockUser_;
    TestFakeSockPeer peer_;
};

class TestRosterUser : public I_RosterUser {
  public:
    TestRosterUser() : gotDisposing_( false ) {}
    virtual void disposing( I_Roster * roster ) { gotDisposing_ = true; }
    virtual bool validatePeer( I_Roster * roster, sockaddr_in const & peer, 
        char const * name, char const * password ) {
      return (!strcmp( name, "root" ) || !strcmp( name, "guest" )) && !strcmp( password, "password" );
    }
    virtual void userTimeout( I_Roster * roster, sockaddr_in const & peer ) {
    }
    virtual void registerRecord( I_Roster * roster, sockaddr_in const & peer, 
        char const * record, int token ) {
      roster->addRecord( peer, record, token );
    }
    virtual int matchRecords( I_Roster * roster, sockaddr_in const & peer, 
        char const * key, int id, MatchedRecord * outMatches, int maxMatches ) {
      return roster->matchRecords( key, outMatches, maxMatches, 0.75f );
    }
    
    virtual void allowIntroduction( I_Roster * roster, sockaddr_in const & peer1, 
        sockaddr_in const & peer2 ) {
      roster->introduce( peer1, peer2 );
    }
    
    bool gotDisposing_;
};

class TestRosterConnectionNotify : public I_RosterConnectionNotify {
  public:
    TestRosterConnectionNotify() : peer_( 0 ) {}
    void disposing( I_RosterQuery * query ) {
      if( peer_ ) peer_->dispose();
      peer_ = 0;
    }
    ~TestRosterConnectionNotify() {
      if( peer_ ) peer_->dispose();
      peer_ = 0;
    }
    void onPeerConnection( I_RosterQuery * query, 
        I_Socket * socket, I_SocketPeer * peer ) {
      peer_ = peer;
    }
    void onRosterTimeout( I_RosterQuery * query,
        I_Socket * socket ) {
      assert( !"not implemented" );
    }
    I_SocketPeer * peer_;
};

class TestNetwork : public I_Network, public TestNetworkBase {
  public:
    TestNetwork() { tfs_.peer_.nbase_ = this; }
    void addReply( int code, sockaddr_in const & addr, Message const & m ) {
      Reply r;
      r.code = code; r.addr = addr; r.m = m;
      possibleReplies_.push_back( r );
      Reply * rp = &*possibleReplies_.rbegin();
      assert( rp->m.buf_[0] == m.buf_[0] );
    }
    virtual void dispose() {}
    virtual I_Socket * createSocket( unsigned short port, I_SocketUser * user ) {
      return &tfs_;
    }
    virtual void step( double sleepSeconds ) {
      if( replies_.size() ) {
        std::list< Reply >::iterator i = replies_.begin();
        Reply & r = *i;
        if( tfs_.gotCreatePeer_ ) {
          tfs_.peer_.user_->onMessage( &tfs_.peer_, r.m.buf_, r.m.size_ );
        }
        else {
          tfs_.sockUser_->onConnection( &tfs_, r.addr, r.m.buf_, r.m.size_ );
        }
        replies_.erase( i );
      }
    }
    virtual void onMessageSent( char const * buf, size_t size ) {
      unsigned char ch = *(unsigned char *)buf;
      for( std::list< Reply >::iterator ptr = possibleReplies_.begin();
          ptr != possibleReplies_.end();
          ++ptr ) {
        if( ch == (*ptr).code ) {
          replies_.push_back( *ptr );
        }
      }
    }
    struct Reply {
      int code;
      sockaddr_in addr;
      Message m;
    };
    TestFakeSock tfs_;
    std::list< Reply > replies_;
    std::list< Reply > possibleReplies_;
};

}

#endif  //  test_fakes_h
