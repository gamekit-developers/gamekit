
#if !defined( message_h )
#define message_h

#include "network.h"    //  for MAX_MESSAGE_SIZE
#include <assert.h>

namespace libintroduce {

//  This is a simple utility class for generating network packets.
//
//  Usage:
//
//  Message m;
//  m.put( 1 );
//  m.put( 2.3f );
//  m.put( "this is a string" );
//  sendMessage( m.buf_, m.size_ );
//
//  void Me::receive( void const * buf, int size ) {
//    Message m2( buf, size );
//    int i; m2.get( &i );
//    float f; m2.get( &f );
//    char str[ 128 ]; m2.get( str, sizeof( str ) );
//  }
//
//  Note that there is no error checking (except you can't overrun 
//  any buffers; all lengths are checked). Thus, you have to match 
//  up input and output fields/sizes yourself.

class Message {
  public:
    Message();
    Message( void const * src, int len );
    Message( Message const & m );
    Message & operator=( Message const & m );
    ~Message() {}
    void clear();

    bool putn( int num, void const * buf ) { return slurp( buf, num ); }
    //  You never want to send pointers, so if you pass a pointer, you meant the pointed-at thing
    template< typename T > bool put( T const & t ) { return putn( sizeof(T), &t ); }
    template< typename T > bool put( T const * t ) { return putn( sizeof(T), t ); }
    template<> bool put< char >( char const * str ) { return slurp( str, int(strlen( str )) + 1 ); }

    bool getn( int num, void * buf ) { return spew( buf, num ); }
    template< typename T > bool get( T & t ) { return getn( sizeof(T), &t ); }
    // just don't get() on a pointer to a single char (because that looks like a string, which needs buffer size)
    template< typename T > bool get( T * t ) { assert(sizeof(T) != 1); return getn( sizeof(T), t ); }
    bool get( char * buf, int max ) { return spew_cstr( buf, max ); }

    bool prependn( int num, void const * buf );
    template< typename T > bool prepend( T const & t ) { return prependn( sizeof(T), &t ); }
    template< typename T > bool prepend( T const * t ) { return prependn( sizeof(T), t ); }

    //  The semantics of buf_, size_ and base_ are fixed -- you can bang these 
    //  values yourself if you want to, say, skip over bytes when reading.
    //  buf_+base_ is the next place to read from; buf_+size_ is the end of 
    //  the buffer; buf_ is the first valid byte in the buffer, which may be 
    //  somewhere into storage_, if I have space left for copy-less prepend().
    char * buf_;
    int size_;
    int base_;
    //  put storage at the end for cache locality of the other members
    char storage_[ MAX_MESSAGE_SIZE ];

  protected:
    bool slurp( void const * src, int cnt );
    bool spew( void * buf, int len );
    bool spew_cstr( char * buf, int max );
    void copy( Message const & m );
};

}

#endif

