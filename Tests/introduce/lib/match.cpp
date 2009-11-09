
#include "roster.h"

#include <assert.h>

using namespace libintroduce;


static char const * Match(
  char const * strBeg, 
  char const * strEnd,
  char const * matchBeg,
  char const * matchEnd,
  bool anyStart )
{
  assert( strEnd > strBeg );
  assert( matchEnd > matchBeg );
  while( matchEnd-matchBeg <= strEnd-strBeg ) {
    if( *strBeg == *matchBeg && !strncmp( strBeg, matchBeg, matchEnd-matchBeg ) ) {
      return strBeg + (matchEnd-matchBeg);
    }
    if( !anyStart ) {
      break;
    }
    ++strBeg;
  }
  return false;
}

static bool MatchPattern(
  char const * patBeg,
  char const * patEnd,
  char const * strBeg,
  char const * strEnd )
{
  bool anyStart = false;
  char const * matchBeg = patBeg;
  while( patBeg < patEnd ) {
    if( *patBeg == '%' ) {
      if( matchBeg < patBeg ) {
        strBeg = Match( strBeg, strEnd, matchBeg, patBeg, anyStart );
        if( !strBeg ) {
          return false;
        }
      }
      anyStart = true;
      ++patBeg;
      matchBeg = patBeg;
    }
    else {
      ++patBeg;
    }
  }
  if( matchBeg < patBeg ) {
    strBeg = Match( strBeg, strEnd, matchBeg, patBeg, anyStart );
    if( !strBeg ) {
      return false;
    }
  }
  return true;
}

static bool getnum( char const ** str, char const * end, long * out )
{
  //  strtol() needs zero termination, so we create a zero-terminated sub-string.
  //  I picked 48 chars because 47 characters is more than needed to represent 
  //  even a long double in decimal. (128 bits / 3.1 bits per digit ~= 42 digits).
  //  This is overly paranoid, as I only support integers anyway.
  char num[ 48 ];
  size_t l = end-*str;
  if( l > 47 ) {
    l = 47;
  }
  memcpy( num, *str, l );
  num[ l ] = 0;
  char * o = 0;
  *out = strtol( *str, &o, 10 );
  if( o == 0 || o == *str ) {
    return false;
  }
  *str = o;
  return true;
}

static bool MatchChunk( 
  char const * recBeg, 
  char const * recEnd, 
  char op,
  char const * valBeg,
  char const * valEnd )
{
  if( op == '<' || op == '>' ) {
    //  numeric comparision
    long l1, l2;
    if( !getnum( &recBeg, recEnd, &l1 ) ) {
      return 0;
    }
    if( !getnum( &valBeg, valEnd, &l2 ) ) {
      return 0;
    }
    if( op == '<' && l1 < l2 ) {
      return true;
    }
    if( op == '>' && l1 > l2 ) {
      return true;
    }
  }
  else {
    assert( op == '=' );
    return MatchPattern( valBeg, valEnd, recBeg, recEnd );
  }
  return false;
}

static int AttemptMatch( 
  char const * record, 
  char const * name,
  char const * nameEnd,
  char op,
  char const * val,
  char const * valEnd )
{
  //  pick apart the record to find key/value pairs to match against
  char const * beg = record;
  char const * eq;
  while( true ) {
    eq = strchr( beg, '=' );
    if( !eq ) {
      if( *beg != 0 ) {
        warning( "poorly formatted match record: %s", beg );
      }
      break;
    }
    char const * semi = strchr( eq+1, ';' );
    if( !semi ) {
      semi = eq + strlen( eq );
    }
    if( eq-beg == nameEnd-name && !strncmp( beg, name, eq-beg ) ) {
      if( MatchChunk( eq+1, semi, op, val, valEnd ) ) {
        return 1;
      }
    }
    if( !*semi ) {
      break;
    }
    beg = semi+1;
  }

  return 0;
}

float libintroduce::MatchRecord( char const * record, char const * key )
{
  int nFound = 0;     //  number of key clauses
  int nMatch = 0;     //  number actually matched
  //  pick apart the values in the key
  char const * cur = key;
  char const * end = key;
  while( cur ) {
    end = strchr( cur, ';' );
    if( end == 0 ) {
      end = cur + strlen( cur );
    }
    if( cur == end ) {
      break;
    }
    char const * op = cur;
    while( op < end ) {
      if( *op == '=' || *op == '<' || *op == '>' ) {
        break;
      }
      ++op;
    }
    //  let's assume we found an op
    ++nFound;
    if( op < end ) {
      //  actually attempt match
      nMatch += AttemptMatch( record, cur, op, *op, op+1, end );
    }
    else {
      //  warn on mal-formed key?
    }
    cur = *end ? end+1 : 0;
  }
  assert( nMatch <= nFound );
  if( nFound == 0 ) {
    return 1;
  }
  if( nMatch == 0 ) {
    return 0;
  }
  return float(nMatch)/float(nFound);
}


void Unittest_MatchRecord()
{
  assert( MatchRecord( "", "" ) == 1 );
  assert( MatchRecord( "foo=1", "" ) == 1 );
  assert( MatchRecord( "", "foo=1" ) == 0 );
  assert( MatchRecord( "foo=1", "foo>0" ) == 1 );
  assert( MatchRecord( "foo=1", "foo<0" ) == 0 );
  assert( MatchRecord( "foo=1;bar=1;baz=2", "baz<2" ) == 0 );
  assert( MatchRecord( "foo=1;bar=1;baz=2", "baz>1" ) == 1 );
  assert( MatchRecord( "foo=1;bar=1;baz=2", "baz>1;bar=0" ) == 0.5f );
  assert( MatchRecord( "foo=this is a string;bar=another string", "foo=%another%" ) == 0 );
  assert( MatchRecord( "foo=this is a string;bar=another string", "foo=%string%;bar=%this%" ) == 0.5f );
  assert( MatchRecord( "foo=this is a string;bar=another string", "foo=%string%;bar=%string%" ) == 1 );
  assert( MatchRecord( "foo=this is a string;bar=another string", "bar=another %" ) == 1 );
  assert( MatchRecord( "foo=this is a string;bar=another string", "bar=%another string" ) == 1 );
  assert( MatchRecord( "foo=this is a string;bar=another string", "bar=% string" ) == 1 );
  assert( MatchRecord( "foo=this is a string;bar=another string", "foo=%=%" ) == 0 );
}

