
#if !defined( i_rosterconnectionnotify_h )
#define i_rosterconnectionnotify_h

#include "roster.h"

namespace libintroduce {

class I_RosterQuery;

class I_RosterConnectionNotify {
  public:
    //  You will be called back when the roster query goes away.
    virtual void disposing( I_RosterQuery * query ) = 0;
    //  I_RosterQuery calls this callback when a peer tries 
    //  to connect to the socket specified on the roster, after 
    //  the roster has actually connected to the roster server.
    //  It is up to you (the implementer of onPeerConnection()) 
    //  to dispose the I_SocketPeer when you're done with it.
    //  You should typically configure a user on the peer, and 
    //  start trying to communicate.
    virtual void onPeerConnection( I_RosterQuery * query, 
        I_Socket * socket, I_SocketPeer * peer ) = 0;
    //  I_RosterQuery can also tell you about timeouts if there 
    //  are no keepalives received from the registration server 
    //  for a long while.
    virtual void onRosterTimeout( I_RosterQuery * query, 
        I_Socket * socket ) = 0;
};

}

#endif  //  i_rosterconnectionnotify_h
