
#include "message.h"

#include <string.h>
#include <assert.h>

#define PREPEND_PAD 8

using namespace libintroduce;

Message::Message()
{
  clear();
}

Message::Message( void const * src, int len )
{
  if( len > sizeof( storage_ ) ) {
    throw "Too large buffer for Message";
  }
  buf_ = storage_;
  size_ = len;
  base_ = 0;
  memcpy( buf_, src, len );
}

Message::Message( Message const & m )
{
  copy( m );
}

Message & Message::operator=( Message const & m )
{
  copy( m );
  return *this;
}

void Message::copy( Message const & m )
{
  buf_ = &storage_[m.buf_-m.storage_];
  size_ = m.size_;
  base_ = m.base_;
  memcpy( buf_, m.buf_, size_ );
}


void Message::clear()
{
  //  empty messages may later be pre-pended into
  buf_ = &storage_[PREPEND_PAD];
  size_ = 0;
  base_ = 0;
}

bool Message::slurp( void const * src, int cnt )
{
  if( cnt > int(sizeof(storage_)-size_) ) {
    return false;
  }
  //  shift down if we need to make room
  if( buf_+size_+cnt > &storage_[MAX_MESSAGE_SIZE] ) {
    assert( buf_ > storage_ );
    memmove( storage_, buf_, size_ );
    buf_ = storage_;
  }
  memcpy( &buf_[size_], src, cnt );
  size_ += cnt;
  return true;
}

bool Message::spew( void * buf, int len )
{
  if( len > size_-base_ ) {
    return false;
  }
  memcpy( buf, &buf_[base_], len );
  base_ += len;
  return true;
}

bool Message::spew_cstr( char * buf, int max )
{
  assert( base_ <= size_ );
  if( base_ == size_ ) {
    *buf = 0;
    return false;
  }
  //  find amount of space, excluding terminator
  int l = max-1;
  if( l + base_ >= size_ ) {
    l = size_-base_-1;
  }
  //  find length of string within these limits
  int n = 0;
  while( n < l ) {
    if( buf_[base_+n] == 0 ) {
      break;
    }
    ++n;
  }
  //  if string is not terminated, complain
  if( buf_[base_+n] ) {
    *buf = 0;
    return false;
  }
  memcpy( buf, &buf_[base_], n+1 );
  base_ += n+1;
  return true;
}

bool Message::prependn( int num, void const * src )
{
  if( num > int(sizeof(storage_)-size_) ) {
    return false;
  }
  //  if there's size in the prepend area, then use that
  if( buf_-storage_ >= num ) {
    buf_ -= num;
    memcpy( buf_, src, num );
    size_ += num;
    if( base_ > 0 ) {
      base_ += num;
    }
    return true;
  }
  //  compact the message
  memmove( &storage_[num], buf_, size_ );
  buf_ = storage_;
  memcpy( buf_, src, num );
  size_ += num;
  if( base_ > 0 ) {
    base_ += num;
  }
  return true;
}

void Unittest_Message()
{
  Message m;
  
  memset( &m.storage_, 0, sizeof( m.storage_ ) ); //  to test string termination below
  m.put( 1 );
  m.put( 2.25f );
  m.put( "hello, world" );
  assert( m.size_ == 4+4+13 );
  int i; m.get( &i ); assert( i == 1 );
  float f; m.get( &f ); assert( f == 2.25f );
  char buf[ 32 ]; m.get( buf, 32 ); assert( !strcmp( buf, "hello, world" ) );
  assert( !m.get( buf, 32 ) );  //  out of data
  m.prependn( 4, "1234" );
  m.base_ = 0;
  buf[0] = 0;
  m.getn( 4, buf );
  assert( !strncmp( buf, "1234", 4 ) );
  m.base_ = m.size_;

  Message m2 = m;
  m.base_ = 0;
  m.size_ = 0;
  assert( m2.size_ == 4+4+4+13 );
  assert( m2.base_ == m2.size_ );

  char temp[ MAX_MESSAGE_SIZE ];
  assert( m.putn( sizeof(temp)-4, temp ) );
  assert( !m.put( "hello" ) );
  assert( m.put( "hey" ) );
  assert( m.size_ == sizeof( temp ) );
  assert( !m.prependn( 1, "" ) );

  Message m3( m.storage_, m.size_ );
  assert( m.getn( sizeof(temp)-4, temp ) );
  temp[4] = 0x45;
  assert( m.get( temp, 100 ) );
  assert( !strcmp( temp, "hey" ) );
  assert( temp[4] == 0x45 );
}

