#if !defined( i_socket_h )
#define i_socket_h

#include "sock_port.h"

namespace libintroduce {

class I_Network;
class I_SocketUser;
class I_SocketPeer;
class I_SocketPeerUser;

class I_Socket {
public:
  //  When you're done with me (the socket), dispose me.
  virtual void dispose() = 0;
  //  I know which Network instance I belong to.
  virtual I_Network * network() = 0;
  //  I can tell you the user that was configured on me on creation.
  virtual I_SocketUser * socketUser() = 0;
  //  You can reconfigure my user.
  virtual void setSocketUser( I_SocketUser * user ) = 0;
  //  I can create a peer for you. It is an error to create a peer 
  //  for an address that already has a peer; such attempts will return NULL.
  virtual I_SocketPeer * createPeer( sockaddr_in const & addr, I_SocketPeerUser * user ) = 0;
  //  I can create a peer using text-format addressing, too (this may 
  //  not be instantaneous, because DNS is synchronous). If the address 
  //  does not contain a :port part, I use the port of my socket.
  virtual I_SocketPeer * createPeer( char const * addr, I_SocketPeerUser * user, 
      char * oError, int eSize ) = 0;
  //  return the peer for an address, if available
  virtual I_SocketPeer * findPeer( sockaddr_in const & addr ) = 0;
  //  I can send a message to an arbitrary address, if you want. It may or 
  //  may not make it to the other end; there's no way to tell.
  virtual void sendMessage( sockaddr_in const & addr, void const * buf, int bSize ) = 0;
  //  I can tell you the local name of the socket. This may be of 
  //  limited utility, because it's likely to be a private address, 
  //  not visible on the global internet. That's why you should use 
  //  the Roster interface for finding other peers to talk to.
  virtual sockaddr_in address() = 0;
};

}

#endif  //  i_socket_h
