
#if !defined( sort_addr_h )
#define sort_addr_h

#include "sock_port.h"

namespace libintroduce {

//  sort_sockaddr_in is a convenient wrapper around sockaddr_in, that will 
//  allow you to use it as a key for a std::map<>. The ordering is defined 
//  to be stable, but not necessarily otherwise useful in any specific way 
//  (because of the network byte order of data).

struct sort_sockaddr_in : public sockaddr_in {
  sort_sockaddr_in() { memset( this, 0, sizeof( *this ) ); }
  sort_sockaddr_in( sockaddr_in const & o ) : sockaddr_in( o ) {}
  //  I don't need to care about network/host byte order here 
  //  because all sort_sockaddr_in structs are in the same format.
  bool operator==( sockaddr_in const & o ) const {
    return SIN_ADDR_UINT( sin_addr ) == SIN_ADDR_UINT( o.sin_addr ) && 
        sin_port == o.sin_port;
  }
  bool operator<( sockaddr_in const & o ) const {
    return SIN_ADDR_UINT( sin_addr ) < SIN_ADDR_UINT( o.sin_addr ) ||
        (SIN_ADDR_UINT( sin_addr ) == SIN_ADDR_UINT( o.sin_addr ) && 
            sin_port < o.sin_port);
  }
  char const * staticText() const {
    static char buf[ 64 ];
    unsigned long ul = htonl( SIN_ADDR_UINT(sin_addr) );
    unsigned short us = htons( sin_port );
    sprintf( buf, "%d.%d.%d.%d:%d", ul>>24, (ul>>16)&0xff, (ul>>8)&0xff, ul&0xff, us );
    return buf;
  }
};

}

#endif  //  sort_addr_h

