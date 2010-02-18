
#include "network.h"
#include "roster.h"
#include "clock.h"
#include "sort_addr.h"
#include "xfer_proto.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

#include <string>
#include <list>
#include <map>


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


#define PEER_CONNECT_RETRY 1.0

struct FileInfo {
  std::string     path;
  std::string     name;
  fpos_t          size;
  bool            registered;
  int             id;
  int             fd;
  int             fdRefCount;
};

std::string gUserName = "luser";
std::string gPassword = "secret";
int gPort = 1599;
std::string gServer = "introduce.me:1599";     //  .me is not actually a TLD as of June 2004
std::list< FileInfo > gSharedFiles;
volatile bool gDumpInfo = false;
bool gWaitForReturn;

I_Network * gNetwork;
I_Clock * gClock;
I_RosterQuery * gRosterQuery;
I_Socket * gSocket;

//  Print a helpful command-line usage message, and quit in disgust.
void usage()
{
  fprintf( stderr, "Usage: putter [options] file ...\n" );
  fprintf( stderr, "This program will make given files available for peer-to-peer\n" );
  fprintf( stderr, "sharing, assuming a roster server is available at the roster\n" );
  fprintf( stderr, "address given. This program can be run behind a NAT firewall.\n" );
  fprintf( stderr, "Long name    Short   Default\n" );
  fprintf( stderr, "--user        -u     %s\n", gUserName.c_str() );
  fprintf( stderr, "--password    -w     %s\n", gPassword.c_str() );
  fprintf( stderr, "--port        -p     %d  (local port)\n", gPort );
  fprintf( stderr, "--server      -s     %s\n", gServer.c_str() );
  fprintf( stderr, "--wait        -z     wait for return before starting\n" );
  exit( 1 );
}

void addFileName( char const * name )
{
  FileInfo fi;
  fi.path = name;
  char const * s = strrchr( name, '/' );
  if( s ) ++s; else s = name;
  fi.name = s;
  stat64struct st;
  if( stat64func( name, &st ) < 0 ) {
    fprintf( stderr, "Cannot find file: %s\n", name );
    exit( 2 );
  }
  fi.size = st.st_size;
  fi.registered = false;
  fi.id = int(1 + gSharedFiles.size());
  fi.fd = -1;
  fi.fdRefCount = 0;
  gSharedFiles.push_back( fi );
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
  if( gSharedFiles.size() == 0 ) {
    fprintf( stderr, "ERROR: you must share at least one file!\n" );
    usage();
  }
}

void setDumpInfo( int sig )
{
  gDumpInfo = true;
}

void installSignalHandlers()
{
  signal( SIGHUP, setDumpInfo );
}

void warnFunc( int level, char const * str, va_list vl )
{
  vfprintf( stderr, str, vl );
  static FILE * f = fopen( "putter.log", "wb" );
  vfprintf( f, str, vl );
  fflush( f );
}

namespace {
  class RosterConnectionNotify : public I_RosterConnectionNotify {
    public:
      RosterConnectionNotify() {
        timeout_ = false;
      }
      virtual void disposing( I_RosterQuery * query ) {
      }
      virtual void onPeerConnection( I_RosterQuery * query, 
          I_Socket * socket, I_SocketPeer * peer ) {
        log( "RosterConnectionNotify::onPeerConnection() 0x%lx peer %s\n", this, 
            sort_sockaddr_in( peer->peerAddress() ).staticText() );
        assert( query == gRosterQuery );
        assert( socket == gSocket );
        Peer p( peer );
        peers_[p.addr_] = p;
      }
      void onRosterTimeout( I_RosterQuery * query, 
          I_Socket * socket ) {
        log( "RosterConnectionNotify::onRosterTimeout() 0x%lx\n", this );
        assert( query == gRosterQuery );
        assert( socket == gSocket );
        timeout_ = true;
      }
      void step( double now ) {
        std::map< sort_sockaddr_in, Peer >::iterator ptr = peers_.begin();
        while( ptr != peers_.end() ) {
          Peer & p = (*ptr).second;
          if( !p.connected_ && p.lastSentTime_ < now ) {
            //  try to send again
            assert( !"not implemented" );
            p.lastSentTime_ = now + PEER_CONNECT_RETRY;
          }
          ++ptr;
        }
      }

      struct Peer {
        Peer() {}
        Peer( I_SocketPeer * peer ) : peer_( peer ), addr_( peer->peerAddress() ), 
            connected_( false ), connectCount_( 0 ), lastReceivedTime_( 1e-15 ),
            lastSentTime_( 1e-15 ) {}
        I_SocketPeer * peer_;
        sort_sockaddr_in addr_;
        bool connected_;
        int connectCount_;
        double lastReceivedTime_;
        double lastSentTime_;
      };
      std::map< sort_sockaddr_in, Peer > peers_;
      bool timeout_;
  };
  
  RosterConnectionNotify gRosterConnectionNotify;
  
  char const * itoa( int i ) {
    static char buf[ 40 ];
    sprintf( buf, "%d", i );
    return buf;
  }

  char const * lltoa( int64 ll ) {
    static char buf[ 60 ];
    sprintf( buf, LLFMT, ll );
    return buf;
  }
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

void addAllFiles()
{
  //  Add all files to the roster. Note that it's OK to do this before 
  //  the roster is actually connected; the roster will remember the fact 
  //  that we want to register these keys, and register them once it 
  //  connects to the introducer/matchmaker.
  std::list< FileInfo >::iterator curFile = gSharedFiles.begin();
  while( curFile != gSharedFiles.end() ) {
    std::string s = "name=";
    s += (*curFile).name;
    s += ";size=";
    s += lltoa( (*curFile).size );
    //  Adding the Id is convenient for debugging; not really necessary for
    //  the application.
    s += ";id=";
    s += itoa( (*curFile).id );
    gRosterQuery->add( s.c_str(), (*curFile).id );
    ++curFile;
  }
}

void dumpInfo()
{
  gDumpInfo = false;
  fprintf( stderr, "---- PUTTER DUMP INFO ----\n" );
  fprintf( stderr, "Current time: %.2f\n", gClock->realTime() );
  fprintf( stderr, "Num files: %d\n", gSharedFiles.size() );
  fprintf( stderr, "Num peers: %d\n", gRosterConnectionNotify.peers_.size() );
  for( std::list< FileInfo >::iterator ptr = gSharedFiles.begin();
      ptr != gSharedFiles.end(); ++ptr ) {
    fprintf( stderr, "File id %d: name=%s; size=%ld; fd=%d; refCount=%d; path=%s\n", 
        (*ptr).id, (*ptr).name.c_str(), long((*ptr).size), (*ptr).fd, (*ptr).fdRefCount, 
        (*ptr).path.c_str() );
  }
  for( std::map< sort_sockaddr_in, RosterConnectionNotify::Peer >::iterator ptr = 
      gRosterConnectionNotify.peers_.begin(); ptr != gRosterConnectionNotify.peers_.end(); 
      ++ptr ) {
    RosterConnectionNotify::Peer & p = (*ptr).second;
    fprintf( stderr, "Peer %s; %s; conn count %d; last send %.2f; last recv %.2f\n", 
        p.addr_.staticText(), p.connected_ ? "connected" : "pending", 
        p.connectCount_, p.lastReceivedTime_, p.lastSentTime_ );
  }
}

void runMainLoop()
{
  int timeoutCnt = 0;
  double then = gClock->realTime();
  while( true ) {
    gNetwork->step( 1 );
    gRosterQuery->step( gClock->realTime() );
    double now = gClock->realTime();
    gRosterConnectionNotify.step( now );
    if( gDumpInfo ) {
      dumpInfo();
    }
    if( now-then > 9.3 ) {
      then = now;
      log( "time %g: %s\n", now, gRosterQuery->connected() ? "connected" : "not connected" );
      if( !gRosterQuery->connected() && (++timeoutCnt > 3) ) {
        warning( "timeout trying to connect\n" );
        break;
      }
      if( gRosterConnectionNotify.timeout_ ) {
        warning( "timeout on roster connection\n" );
        break;
      }
    }
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
  installSignalHandlers();
  initializeNetwork();
  addAllFiles();
  runMainLoop();

  return 0;
}
