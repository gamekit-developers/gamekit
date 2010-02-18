
#if !defined( network_h )
#define network_h

#include <stdarg.h>

#include "sock_port.h"
#include "i_socket.h"
#include "i_socketuser.h"
#include "i_socketpeer.h"
#include "i_socketpeeruser.h"

namespace libintroduce {

//  I implement this interface; you get ahold of an instance by 
//  calling CreateNetwork. When you don't need any networking 
//  services anymore, call I_Network::dispose() on your interface.
//  You only ever need one instance of this interface, as it can 
//  open multiple, independent sockets.
class I_Network {
public:
  //  When you're done, dispose me. This will delete all remaining 
  //  instances created through me, but it will also print warnings 
  //  because this likely means you have a leak in your program.
  virtual void dispose() = 0;
  //  Open a socket on a port to start communicating.
  virtual I_Socket * createSocket( unsigned short port, I_SocketUser * user ) = 0;
  //  Periodically (like, once a game step) you should call step() 
  //  on me. I will check whether there are any incoming messages 
  //  from socket peers, and dispatch them through the User callback 
  //  interfaces. Pass 0 sleepSeconds to just do a poll. Pass something 
  //  bigger to actually sleep if there is no data.
  virtual void step( double sleepSeconds ) = 0;
};


// warning levels passed to the warnFunc
#define WL_WARNING 3
#define WL_LOG 2
#define WL_DEBUG 0
I_Network * CreateNetwork( char * o_error, int eSize, void (*warnFunc)( int level, char const *, va_list ) );

void warning( char const * str, ... );
void log( char const * str, ... );
void debug( char const * str, ... );

}

//  this is the maximum payload I'll let you send or receive
#define MAX_MESSAGE_SIZE (128*11)

#endif  //  network_h
