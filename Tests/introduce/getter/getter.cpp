

#include "network.h"
#include "roster.h"
#include "../putter/xfer_proto.h"
#include "clock.h"
#include "sort_addr.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include <string>
#include <list>
#include <map>
#include <vector>


#if defined( WIN32 )
#include <direct.h>
#include <signal.h>
#define SIGHUP 3
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#define stat64func _stat64
#define stat64struct struct __stat64
#define LLFMT "%Ld"
typedef __int64 int64;
#else
#include <dirent.h>
#include <unistd.h>
#include <sys/signal.h>
typedef long long int64;
#define LLFMT "%lld"
#endif


std::string gUserName = "luser";
std::string gPassword = "secret";
int gPort = 1599;
std::string gServer = "introduce.me:1599";     //  .me is not actually a TLD as of June 2004
bool gRunning = true;
bool gWaitForReturn;

std::list< std::string > gFilesToGet;
bool gGetListOfFiles = true;


I_Network * gNetwork;
I_Clock * gClock;
I_RosterQuery * gRosterQuery;
I_Socket * gSocket;


//  Print a helpful command-line usage message, and quit in disgust.
void usage()
{
  fprintf( stderr, "Usage: getter [options] [file ...]\n" );
  fprintf( stderr, "This program will retrieve the given files from some peer, as\n" );
  fprintf( stderr, "brokered by the given roster server.\n" );
  fprintf( stderr, "If no file given, will list all available files.\n" );
  fprintf( stderr, "This program can run behind a NAT firewall.\n" );
  fprintf( stderr, "long name    short   default\n" );
  fprintf( stderr, "--user        -u     %s\n", gUserName.c_str() );
  fprintf( stderr, "--password    -w     %s\n", gPassword.c_str() );
  fprintf( stderr, "--port        -p     %d  (local port)\n", gPort );
  fprintf( stderr, "--server      -s     %s\n", gServer.c_str() );
  fprintf( stderr, "--wait        -z     wait for return before starting\n" );
  exit( 1 );
}

void addFileName( char const * str )
{
  gFilesToGet.push_back( str );
  gGetListOfFiles = false;
}

//  Parse an option, if available.
int option( int num, char const ** & argv )
{
  if( !strcmp( argv[0], "--user" ) || !strcmp( argv[0], "-u" ) ) {
    if( num < 2 ) {
      usage();
    }
    gUserName = argv[1];
    return 2;
  }
  if( !strcmp( argv[0], "--password" ) || !strcmp( argv[0], "-w" ) ) {
    if( num < 2 ) {
      usage();
    }
    gPassword = argv[1];
    return 2;
  }
  if( !strcmp( argv[0], "--port" ) || !strcmp( argv[0], "-p" ) ) {
    if( num < 2 ) {
      usage();
    }
    gPort = atoi( argv[1] );
    if( gPort < 1 || gPort > 65535 ) {
      fprintf( stderr, "ERROR: --port argument must be in range [1,65535]\n" );
      exit( 1 );
    }
    return 2;
  }
  if( !strcmp( argv[0], "--server" ) || !strcmp( argv[0], "-s" ) ) {
    if( num < 2 ) {
      usage();
    }
    gServer = argv[1];
    return 2;
  }
  if( !strcmp( argv[0], "--wait" ) || !strcmp( argv[0], "-z" ) ) {
    gWaitForReturn = true;
    return 1;
  }
  return 0;
}

//  Parse all available options in argv/argc
void readOptions( int argc, char const ** argv )
{
  int cur = 1;
  ++argv;
  while( cur < argc ) {
    if( !strcmp( *argv, "--" ) || ((*argv)[0] != '-') ) {
      if( **argv == '-' ) {
        ++cur;
        ++argv;
      }
      break;
    }
    int used = option( argc-cur, argv );
    if( used == 0 ) {
      usage();
    }
    cur += used;
    argv += used;
  }
  while( cur < argc ) {
    addFileName( *argv );
    ++argv;
    ++cur;
  }
}

namespace {
  class RosterConnectionNotify : public I_RosterConnectionNotify {
    public:
      RosterConnectionNotify() { timeout_ = false; peer_ = 0; }
      void disposing( I_RosterQuery * ) {}
      void onPeerConnection( I_RosterQuery * query, 
          I_Socket * socket, I_SocketPeer * peer ) {
        peer_ = peer;
      }
      void onRosterTimeout( I_RosterQuery * query, 
          I_Socket * socket ) {
        timeout_ = true;
      }
      I_SocketPeer * peer_;
      bool timeout_;
  };
  RosterConnectionNotify gRosterConnectionNotify;
}

void warnFunc( int level, char const * str, va_list vl )
{
  vfprintf( stderr, str, vl );
  static FILE * f = fopen( "getter.log", "wb" );
  vfprintf( f, str, vl );
  fflush( f );
}

void initializeNetwork()
{
  char err[100];
  gNetwork = CreateNetwork( err, 100, warnFunc );
  if( !gNetwork ) {
    fprintf( stderr, "Can't create network: %s\n", err );
    exit( 2 );
  }
  gClock = CreateClock();
  if( !gClock ) {
    fprintf( stderr, "Can't create clock.\n" );
    exit( 2 );
  }
  gRosterQuery = CreateRosterQuery();
  if( !gRosterQuery ) {
    fprintf( stderr, "Can't create roster query.\n" );
    exit( 2 );
  }
  for( int portAdd = 0; portAdd < 11; ++portAdd ) {
    gSocket = gNetwork->createSocket( gPort+portAdd, 0 );
    if( gSocket ) {
      break;
    }
  }
  if( !gSocket ) {
    fprintf( stderr, "can't bind client on any ports %d - %d\n", gPort, gPort+10 );
    exit( 2 );
  }
  gRosterQuery->notify( &gRosterConnectionNotify );
  gRosterQuery->connect( gSocket, gServer.c_str(), gUserName.c_str(), gPassword.c_str() );
}


void stepNotConnected();
void stepWaitingForReply();
void stepWaitingForConnection();
void stepTransferring();
void stepDone();


enum QueryState {
  QueryStateNotConnected,
  QueryStateWaitingForReply,
  QueryStateWaitingForConnection,
  QueryStateTransferring,
  QueryStateDone,
};

struct {
  char const * name;
  void (*func)();
}
gStateFuncs[] = {
  { "Connecting to match-maker", stepNotConnected },
  { "Querying", stepWaitingForReply },
  { "Connecting to peer", stepWaitingForConnection },
  { "Transferring", stepTransferring },
  { "Done", stepDone },
};
int gState = QueryStateNotConnected;

namespace {
  class C_RosterQueryUser : public I_RosterQueryUser {
    public:
      void disposing( I_RosterQuery * query ) {
        log( "C_RosterQueryUser::disposing()\n" );
        // do nothing
      }
      void onQueryResult( I_RosterQuery * query, 
          MatchedRecord const * matched, int numMatched ) {
        // we're done!
        log( "C_RosterQueryUser::onQueryResult()\n" );
        complete_ = true;
        matches_.insert( matches_.begin(), matched, matched+numMatched );
      }
      void onQueryTimeout( I_RosterQuery * query ) {
        complete_ = true;
        error_ = true;
        log( "C_RosterQueryUser::onQueryTimeout()\n" );
      }
      void clear() {
        complete_ = false;
        error_ = false;
        matches_.swap( std::vector< MatchedRecord >() );
      }
      bool complete() const {
        return complete_;
      }
      bool error() const {
        return error_;
      }
      int numRecords() const {
        return (int)matches_.size();
      }
      MatchedRecord const * records() const {
        return &matches_[0];
      }
    private:
      std::vector< MatchedRecord > matches_;
      bool error_;
      bool complete_;
  };
  C_RosterQueryUser gRosterQueryUser;
}

void stepNotConnected()
{
  if( gRosterQuery->connected() ) {
    log( "connected to roster\n" );
    gState = QueryStateWaitingForReply;
    if( gGetListOfFiles ) {
      gRosterQueryUser.clear();
      gRosterQuery->query( "name=%", &gRosterQueryUser, 100 );
    }
    else {
      std::list< std::string >::iterator f = gFilesToGet.begin();
      std::string q = "name=";
      q += (*f);
      gRosterQueryUser.clear();
      gRosterQuery->query( q.c_str(), &gRosterQueryUser, 10 );
    }
  }
}

void stepWaitingForReply()
{
  if( gRosterQueryUser.error() ) {
    fprintf( stderr, "error querying roster (could we connect?)\n" );
    gRunning = false;
    return;
  }
  if( gRosterQueryUser.complete() ) {
    if( gGetListOfFiles ) {
      MatchedRecord const * mr = gRosterQueryUser.records();
      int n = gRosterQueryUser.numRecords();
      for( int i = 0; i < n; ++i ) {
        fprintf( stdout, "%s\t%s\n", mr[i].record, mr[i].score, 
            sort_sockaddr_in( mr[i].addr ).staticText() );
      }
      gRunning = false;
    }
    else {
      int n = gRosterQueryUser.numRecords();
      if( n == 0 ) {
        fprintf( stderr, "no matching file was found\n" );
        gRunning = false;
        return;
      }
      int ix = (rand() & 0x7fff) % n;
      sort_sockaddr_in ssin = gRosterQueryUser.records()[ix].addr;
      log( "connecting to %s\n", ssin.staticText() );
      gState = QueryStateWaitingForConnection;
      gRosterQuery->introduce( ssin );
    }
  }
  if( gRosterConnectionNotify.timeout_ ) {
    fprintf( stderr, "timeout from roster (after connection)\n" );
    gRunning = false;
    return;
  }
}

namespace {
  class Transfer : public I_SocketPeerUser {
    public:
      Transfer() { file_ = 0; name_ = ""; size_ = 0; complete_ = false; lastRequest_ = 0; }
      ~Transfer() {
        if( file_ ) {
          fclose( file_ );
          _unlink( name_.c_str() );
        }
      }

      void init( char const * file, long size ) {
        char * ptr = strrchr( name_, '/' );
        if( ptr ) ptr++; else ptr = name_;
        char * ptr2 = strrchr( ptr, '\\' );
        if( ptr2 ) ptr = ptr2+1;
        if( strlen( ptr ) == 0 ) {
          ptr = "received.dat";
        }
        name_ = ptr;
        file_ = fopen( ptr, "wb" );
        size_ = size;
        togo_.push_back( std::pair< long, long >( 0, size_ ) );
        complete_ = false;
      }

      void put( void const * buf, long pos, long size ) {
        fseek( file_, pos, 0 );
        fwrite( buf, size, 1, file_ );
        removeSpan( pos, pos+size );
      }

      void done() {
        fclose( file_ );
        file_ = 0;
        peer_ = 0;
        complete_ = true;
      }

      void removeSpan( long a, long b ) {
        std::list< std::pair< long, long > >::iterator ptr = togo_.begin();
        while( ptr != togo_.end() ) {
          std::list< std::pair< long, long > >::iterator next = ptr;
          ++next;
          if( (*ptr).first <= b && (*ptr).second >= a ) {
            //  at least part of the span is covered here
            if( (*ptr).first >= a ) {
              //  cutting from the front
              if( (*ptr).second <= b ) {
                //  totally gone
                togo_.erase( ptr );
              }
              else {
                (*ptr).first = b;
              }
            }
            else if( (*ptr).second <= b ) {
              //  cutting from the back
              assert( (*ptr).first < a ); //  else previous case should have been hit
              (*ptr).second = a;
            }
            else {
              //  cutting in the middle
              std::pair< long, long > second;
              second.first = (*ptr).first;
              second.second = a;
              (*ptr).first = b;
              togo_.insert( ptr, second );
            }
          }
          assert( (*ptr).first < (*ptr).second );
          ptr = next;
        }
      }

      bool nextSpan( long * a, long * b ) {
        if( todo_.size() == 0 ) {
          return false;
        }
        *a = todo_.front().first;
        *b = todo_.front().second;
        return true;
      }

      FILE * file_;
      std::string name_;
      long size_;
      std::list< std::pair< long, long > > togo_;
      I_SocketPeer * peer_;
      bool complete_;
      double lastRequest_;

      void setPeer( I_SocketPeer * peer ) {
        peer_ = peer;
        peer->setUser( this );
      }

      void step( double now ) {
        if( !complete_ ) {
          maybeSendRequest( now );
          maybeSendKeepalive( now );
          maybeRequestNextSpan( now );
        }
      }

      void maybeSendRequest( double now ) {
        if( !file_ && now > lastRequest_ + 0.9f ) {
          lastRequest_ = now;
          assert( peer_ );
          Message m;
          m.put( gFilesToGet.front().c_str() );
          ProtoHeader::prepend( XferRequest, m );
          peer_->sendMessage( m.buf_, m.size_ );
        }
      }

      void maybeSendKeepalive( double now ) {
        //  this likely never happens, as we keep sending requests faster than the timeout
        if( now > lastRequest_ + 7 ) {
          lastRequest_ = now;
          ProtoHeader ph;
          ph.type = Keepalive;
          ph.size = 0;
          peer_->sendMessage( &ph, sizeof( ph ) );
        }
      }

      void maybeRequestNextSpan( double now ) {
        if( !file_ ) {
          return;
        }
        long a, b;
        if( !nextSpan( &a, &b ) ) {
          disconnect();
        }
        else if( now > lastRequest_ + 2 ) {
          lastRequest_ = now;
          requestNextSpan( a, b );
        }
      }

      void disconnect() {
        XferDisconnect...
        done();
      }

      void requestNextSpan( long a, long b ) {
        XferPacketRequest...
      }

      ... do the peer user, deal with packets ...
  };
  Transfer gTransfer;
}

void stepWaitingForConnection()
{
  I_SocketPeer * p = gRosterConnectionNotify.peer_;
  if( p ) {
    log( "got a peer: %s\n", sort_sockaddr_in( p->peerAddress() ).staticText() );
    gState = QueryStateTransferring;
  }
}

void stepTransferring()
{
  if( !gTransfer.peer_ ) {
    gTransfer.setPeer( gRosterConnectionNotify.peer_ );
  }
  gTransfer.step();
  if( gTransfer.complete_ ) {
    gState = QueryStateDone;
  }
}

void stepDone()
{
  if( gFilesToGet.size() ) {
    gFilesToGet.erase( gFilesToGet.begin() );
    gState = QueryStateNotConnected;
  }
  else {
    gRunning = false;
    log( "All files transferred; done.\n" );
  }
}

void runMainLoop()
{
  while( gRunning ) {
    gNetwork->step( 0.25 );
    gRosterQuery->step( gClock->realTime() );
    (*gStateFuncs[gState].func)();
  }
}

void sleep10() {
  ::Sleep( 5000 );
}

int main( int argc, char const ** argv )
{
  atexit( sleep10 );
  readOptions( argc, argv );
  if( gWaitForReturn ) {
    fprintf( stderr, "Press RETURN to continue:\n" );
    char line[ 10 ];
    fgets( line, 10, stdin );
  }
  initializeNetwork();
  runMainLoop();
  return 0;
}

