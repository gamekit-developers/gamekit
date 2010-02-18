
#include "network.h"
#include "roster.h"
#include "sort_addr.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

#if defined( WIN32 )
#include <signal.h>
#include <direct.h>
#define SIGHUP 3
#else
#include <signal.h>
#include <sys/signal.h>
#include <unistd.h>
#endif

#include <string>
#include <map>


//  Global variables are your friend in small programs like these.
//  Note that in a high-secure environment (which this is NOT), you 
//  don't want to be storing plain-text passwords, you don't want 
//  your protocol to send plain-text passwords, and you don't even 
//  want to store user information in memory for long, because it 
//  may get paged out to disk. This app isn't all that, and doesn't 
//  pretend to be.
char const * gUserFile = "users.csv";           //  where to read user data
int gPort = 1599;                               //  what port to listen on
volatile bool gRescanUsers = true;              //  set when we receive SIGHUP, to re-read user file
I_Network * gNetwork;                           //  the network object (socket factory)
I_Clock * gClock;                               //  the passage of time
I_Roster * gRoster;                             //  the main work-horse of this program
std::map< std::string, std::string > gUsers;    //  the list of passwords keyed by user from user file
#if !defined( NDEBUG )
bool gSpawnDebug;                               //  start getter and putter processes
#endif
bool gWaitForReturn;                            //  wait for return before starting program

//  Use an anonymous namespace to avoid clashing with any other classes
//  of the same name. This probably isn't necessary for this application, 
//  but good form in general.
namespace {
  //  The Roster does most of the work, but calls through this interface 
  //  to allow us to customize operation (limit what certain users can 
  //  do, limit number of connections from the same IP, or whatever). I 
  //  don't particularly want to modify anything, so I just turn around 
  //  and let the roster do as much as it can.
  class RosterUser : public I_RosterUser {
    public:
      RosterUser() {}
      void disposing( I_Roster * roster )
      {
      }
      bool validatePeer( I_Roster * roster, sockaddr_in const & peer, 
          char const * name, char const * password )
      {
        //  Look up the give user and password in the map we built from the user file
        std::string uname( name );
        std::string upass( password );
        std::map< std::string, std::string >::iterator ptr = gUsers.find( uname );
        if( ptr == gUsers.end() ) {
          warning( "user '%s' not found in user file %s\n", name, gUserFile );
          return false;
        }
        sort_sockaddr_in ssin( peer );
        warning( "user '%s' logged in at %s\n", name, ssin.staticText() );
        return true;
      }
      void userTimeout( I_Roster * roster, sockaddr_in const & peer )
      {
        //  I don't care about userTimeout events from the roster object
        sort_sockaddr_in ssin( peer );
        log( "user timeout for %s\n", ssin.staticText() );
      }
      void registerRecord( I_Roster * roster, sockaddr_in const & peer, 
          char const * record, int token )
      {
        sort_sockaddr_in ssin( peer );
        log( "register record for %s; token %d; record '%s'\n", ssin.staticText(), token, record );
        //  turn around and let the roster store this record
        roster->addRecord( peer, record, token );
      }
      int matchRecords( I_Roster * roster, sockaddr_in const & peer, 
          char const * key, int id, MatchedRecord * outMatches, int maxMatches )
      {
        sort_sockaddr_in ssin( peer );
        log( "match records for %s; id %d\n", ssin.staticText(), id );
        //  turn around and let the roster match this record
        return roster->matchRecords( key, outMatches, maxMatches, 0.6f );
      }
      void allowIntroduction( I_Roster * roster, sockaddr_in const & peer1, 
          sockaddr_in const & peer2 )
      {
        sort_sockaddr_in ssin1( peer1 );
        sort_sockaddr_in ssin2( peer2 );
        char buf[ 64 ]; strncpy( buf, ssin1.staticText(), 64 ); buf[63] = 0;
        log( "allow introduction for %s and %s\n", buf, ssin2.staticText() );
        //  turn around and let the roster introduce these peers to each other
        roster->introduce( peer1, peer2 );
      }
  };
  //  The single gRosterUser instance is what we need to create the Roster.
  RosterUser gRosterUser;
}


//  Print a usage message and exit.
void usage()
{
  fprintf( stderr, "ROSTER -- a NAT punch-through introduction server.\n" );
  fprintf( stderr, "This server must be run on a machine not behind a firewall,\n" );
  fprintf( stderr, "or you must make sure it's properly port-forwarded.\n" );
  fprintf( stderr, "Available options:\n" );
  fprintf( stderr, "Long name  Short  Default\n" );
  fprintf( stderr, " --users    -u    %s  (user,password file)\n", gUserFile );
  fprintf( stderr, " --port     -p    %d  (service port)\n", gPort );
  fprintf( stderr, " --wait     -z    wait for return from user before starting\n" );
#if !defined( NDEBUG )
  fprintf( stderr, " --yo       -y    (debug only) spawn a getter and a putter, too\n" );
#endif
  exit( 1 );
}

//  Parse an option, if available.
int option( int num, char const ** & argv )
{
  if( !strcmp( argv[0], "--users" ) || !strcmp( argv[0], "-u" ) ) {
    if( num < 2 ) {
      usage();
    }
    gUserFile = argv[1];
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
#if !defined( NDEBUG )
  if( !strcmp( argv[0], "--yo" ) || !strcmp( argv[0], "-y" ) ) {
    gSpawnDebug = true;
    return 1;
  }
#endif
  if( !strcmp( argv[0], "--wait" ) || !strcmp( argv[0], "-w" ) ) {
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
    //  how many items were used?
    int used = option( argc-cur, argv );
    //  if none, syntax was wrong; kvetch and bail
    if( used == 0 ) {
      usage();
    }
    //  move past to next option
    cur += used;
    argv += used;
  }
}

//  setRescanUsers is the signal handler for SIGHUP; it arranges for 
//  the user file to get re-scanned as soon as the network un-blocks 
//  (which happens because of the signal delivery).
void setRescanUsers(int)
{
  gRescanUsers = true;
}

//  on start-up, install the necessary signal handlers
void installSignals()
{
  signal( SIGHUP, setRescanUsers );
}

//  When creating the network, it needs a function to vector warning 
//  messages to. I just print to stderr.
void warnFunc( int level, char const * str, va_list vl )
{
  vfprintf( stderr, str, vl );
  static FILE * f = fopen( "roster.log", "wb" );
  vfprintf( f, str, vl );
  fflush( f );
}

//  Set up the network objects we need.
void initializeNetwork()
{
  char err[100];
  gNetwork = CreateNetwork( err, sizeof(err), warnFunc );
  if( !gNetwork ) {
    fprintf( stderr, "ERROR: cannot create network: %s\n", err );
    exit( 2 );
  }
  gClock = CreateClock();
  if( !gClock ) {
    //  I doubt this case actually ever happens
    fprintf( stderr, "ERROR: cannot create clock\n" );
    exit( 2 );
  }
  gRoster = CreateRoster( gClock, gNetwork, gPort, &gRosterUser );
  if( !gRoster ) {
    fprintf( stderr, "ERROR: cannot create roster\n" );
    exit( 2 );
  }
}

//  The user file has lines of the form:
//  username,password
//  A line that's empty, or starts with a hash mark (#) is ignored.
//  Comma (,) is not a valid character in a user name.
void rescanUsers()
{
  //  don't scan again, unless we get a signal while reading
  gRescanUsers = false;
  char line[256];
  //  nuke old users, if any (on SIGHUP)
  gUsers.clear();
  //  open up the user list
  FILE * f = fopen( gUserFile, "rb" );
  if( f == 0 ) {
    char cwd[256];
#if defined( WIN32 )
    _getcwd( cwd, 256 ); cwd[255] = 0;
#else
    getcwd( cwd, 256 );
#endif
    fprintf( stderr, "ERROR: user file '%s' not found (cwd: %s). Roster server exiting.\n",
        gUserFile, cwd );
    exit( 2 );
  }
  //  parse out users, skipping comments and empty lines
  int lineNo = 0;
  while( true ) {
    char * user, * password;
    line[0] = 0;
    fgets( line, sizeof(line), f );
    if( !line[0] ) {
      break;  //  end of file
    }
    ++lineNo;
    //  remove newlines
    char * nl = strchr( line, '\n' );
    if( nl ) *nl = 0;
    nl = strchr( line, '\r' );
    if( nl ) *nl = 0;
    //  empty lines, and lines STARTING with a hash mark, are ignored
    if( *line == '#' || !*line ) {
      continue; //  comment
    }
    user = line;
    password = strchr( line, ',' );
    if( !password ) {
      fprintf( stderr, "%s:%d: WARNING: bad line '%s'\n", gUserFile, lineNo, line );
      continue;
    }
    *password = 0; ++password;
    //  remember this user as legit
    gUsers[user] = password;
  }
  fprintf( stderr, "Roster server read %d users from '%s'.\n", gUsers.size(), gUserFile );
}

//  If we got a signal in the previous loop, then re-scan the user list.
//  Make the roster do what it needs, then block waiting for network traffic.
void runMainLoop()
{
  while( true ) {
    if( gRescanUsers ) {
      rescanUsers();
    }
    gRoster->step();
    // Why wake up every 5 seconds? Well, we want to time out inactive users, for one.
    gNetwork->step( 5 );
  }
}

#if !defined( NDEBUG )
//  Make the program come to a debuggable halt
void breakpoint()
{
  *(volatile char *)0;
}

char const * SEError( int e )
{
  switch( e ) {
  case ERROR_FILE_NOT_FOUND: return "The specified file was not found. ";
  case ERROR_PATH_NOT_FOUND: return "The specified path was not found. ";
  case ERROR_BAD_FORMAT: return "The .exe file is invalid (non-Microsoft Win32® .exe or error in .exe image). ";
  case SE_ERR_ACCESSDENIED: return "The operating system denied access to the specified file. ";
  case SE_ERR_ASSOCINCOMPLETE: return "The file name association is incomplete or invalid. ";
  case SE_ERR_DDEBUSY: return "The Dynamic Data Exchange (DDE) transaction could not be completed because other DDE transactions were being processed. ";
  case SE_ERR_DDEFAIL: return "The DDE transaction failed. ";
  case SE_ERR_DDETIMEOUT: return "The DDE transaction could not be completed because the request timed out. ";
  case SE_ERR_DLLNOTFOUND: return "The specified dynamic-link library (DLL) was not found. ";
  case SE_ERR_NOASSOC: return "There is no application associated with the given file name extension. This error will also be returned if you attempt to print a file that is not printable. ";
  case SE_ERR_OOM: return "There was not enough memory to complete the operation. ";
  case SE_ERR_SHARE: return "A sharing violation occurred. ";
  }
  return "No error";
}

//  Spawn a getter and putter that we can use to debug the roster
void spawnDebug()
{
  char cwd[300];
  _getcwd( cwd, 256 );
  cwd[255] = 0;
  fprintf( stderr, "cwd = %s\n", cwd );
  char dir[256];
  strcpy( dir, cwd );
  char * e = strrchr( cwd, '\\' );
  char * de = &dir[e-cwd];
  int r;

#pragma warning( push )
#pragma warning( disable: 4311 )

  strcpy( de, "\\getter" );
  strcpy( e, "\\getter\\debug\\getter.exe" );
  fprintf( stderr, "starting %s in %s...\n", cwd, dir );
  r = (int)::ShellExecute( 0, 0, cwd, "-s localhost:1599 -p 1600 -u luser -w secret -z file.txt", dir, SW_SHOW );
  fprintf( stderr, "getter returned %d (%s)\n", r, SEError( r ) );

  strcpy( de, "\\putter" );
  strcpy( e, "\\putter\\debug\\putter.exe" );
  fprintf( stderr, "starting %s in %s...\n", cwd, dir );
  r = (int)::ShellExecute( 0, 0, cwd, "-s localhost:1599 -p 1611 -u luser -w secret -z file.txt", dir, SW_SHOW );
  fprintf( stderr, "putter returned %d (%s)\n", r, SEError( r ) );

#pragma warning( pop )

  fprintf( stderr, "starting roster...\n" );
}
#endif

int main( int argc, char const ** argv )
{
#if !defined( NDEBUG )
  //  In debug mode, I want to know why the program exits.
  atexit( breakpoint );
#endif
  readOptions( argc, argv );
  if( gWaitForReturn ) {
    fprintf( stderr, "Press RETURN to continue:\n" );
    char line[ 10 ];
    fgets( line, 10, stdin );
  }
#if !defined( NDEBUG )
  //  In debug mode, we might want to start a troika of processes
  if( gSpawnDebug ) {
    spawnDebug();
  }
#endif
  installSignals();       //  SIGHUP means rescan users
  initializeNetwork();    //  create the socket, etc
  runMainLoop();          //  read and process packets
  return 0;
}
