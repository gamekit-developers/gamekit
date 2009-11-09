#if !defined( i_socketpeeruser_h )
#define i_socketpeeruser_h

namespace libintroduce {

//  You implement I_SocketPeerUser to get notified 
//  about arriving data for a specific socket.
class I_SocketPeerUser {
  public:
    //  When the peer is deleted, you will get notified.
    virtual void disposing( I_SocketPeer * peer ) = 0;
    //  When data comes in, you are notified.
    virtual void onMessage( I_SocketPeer * peer, char const * buf, int bSize ) = 0;
};

}

#endif  //  i_socketpeeruser_h
