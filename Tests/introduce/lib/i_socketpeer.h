#if !defined( i_socketpeer_h )
#define i_socketpeer_h

#include "sock_port.h"

namespace libintroduce {

class I_SocketPeerUser;

class I_SocketPeer {
public:
  //  Dispose me when you're done with me.
  virtual void dispose() = 0;
  //  I return the address of the peer.
  virtual sockaddr_in const & peerAddress() = 0;
  //  I return the socket I was created on.
  virtual I_Socket * socket() = 0;
  //  I return the peer user configured on me upon creation.
  virtual I_SocketPeerUser * socketPeerUser() = 0;
  //  I can attempt to send data to the other end.
  //  There are no guarantees that the message will make it 
  //  to the other side. The message will be copied (so you 
  //  can delete the underlying buffer) and may not be sent until 
  //  the next time you call step() on the I_Network.
  virtual void sendMessage( void const * buf, int bSize ) = 0;
};

}

#endif  //  i_socketpeer_h

