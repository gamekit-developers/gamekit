#if !defined( i_socketuser_h )
#define i_socketuser_h

#include "sock_port.h"

namespace libintroduce {

class I_Socket;

//  You implement SocketUser, and had to I_Introduce when 
//  creating a socket.
class I_SocketUser {
  public:
    //  When the bound socket goes away, you are called back 
    //  to be notified about it (although you likely already 
    //  initiated the socket death in the first place).
    virtual void disposing( I_Socket * socket ) = 0;

    //  When a message arrives from a socket that does not have 
    //  a peer, this function is called on you. You can turn around 
    //  and call I_Socket::createPeer(), for example. The buffer is 
    //  the packet data.
    virtual void onConnection( I_Socket * socket, sockaddr_in const & addr, 
        char const * buf, int bSize ) = 0;
};

}

#endif  //  i_socketuser_h
