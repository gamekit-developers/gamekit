
#if !defined( i_rosterquery_h )
#define i_rosterquery_h

#include "roster.h"

namespace libintroduce {

struct MatchedRecord;
class I_Network;
class I_Socket;
class I_Roster;
class I_RosterQueryUser;
class I_RosterConnectionNotify;

//  Time-outs aren't really that easy to detect or do something about 
//  through this interface. Not being able to log in can be detected 
//  by you by polling connected() over time, but once connected, if 
//  the roster crashes or shuts down, and doesn't get a Kick message 
//  through to you, there's now way of knowing.
//  Perhaps your best defense is to create a new query and call connect() 
//  over again every so often (every 10 minutes?).
class I_RosterQuery {
public:
  //  Dispose me when you're done with me; I will disconnect if connected.
  virtual void dispose() = 0;
  //  You can only connect() me to one roster at a time. The roster is 
  //  identified using a server string, or a resolved sockaddr_in.
  //  You must not already have created a peer for the server address 
  //  on the socket.
  virtual void connect( I_Socket * socket, char const * server, 
      char const * name, char const * password ) = 0;
  virtual void connect( I_Socket * socket, sockaddr_in const & server, 
      char const * name, char const * password ) = 0;
  //  You may specify a notify interface, into which peers trying 
  //  to connect to your socket will be dispatched.
  virtual void notify( I_RosterConnectionNotify * notify ) = 0;
  //  connected() returns true if I actually have connected to a server.
  virtual bool connected() = 0;
  //  Once connect() is started, you must step() me until connected() returns 
  //  true. You must also step() me after query() has been issued, until the 
  //  query returns a result. It's also a good idea to step() while connected, 
  //  to make sure keep-alives make it through. Make sure curTime increases 
  //  linearly with time.
  virtual void step( double curTime ) = 0;
  //  To issue a query, call query(). The result, if any, will be reported 
  //  to the user. After some timeout, the user may be told the query was 
  //  unsuccessful. You must step() me while a query is outstanding.
  virtual void query( char const * key, I_RosterQueryUser * user, 
      int maxRecords ) = 0;
  //  To register a record when connected, call add().
  //  You will need to step() me every so often so that I can send 
  //  keep-alives to the server.
  virtual void add( char const * key, int recId ) = 0;
  //  To connect to a peer that you found through the matchmaking service, 
  //  call introduce(); if the introduction is successful, the 
  //  I_RosterConnectionNotify you configured in notify() will be told about it.
  virtual void introduce( sockaddr_in const & peer ) = 0;
};

I_RosterQuery * CreateRosterQuery();

//  If you don't want to do anything else while querying, I provide a simple 
//  synchronous function for connecting to a server and running a query, 
//  returning the number of elements found. The network will be stepped until 
//  timeout happens (about 5 seconds) or a result is retrieved. Returns 0 when 
//  there are no results, and -1 if there's a connectivity error.
//  Ownership for the MatchedRecord strings is kept statically by 
//  SimpleRosterQuery until the next call to SimpleRosterQuery.
//  Note that this isn't really terribly useful, because there is no way to 
//  actually connect to the peers of the returned responses, because that needs 
//  the introducer to tell the other guy to cooperate.
int SimpleRosterQuery( I_Network * nw, 
    char const * server, 
    char const * user, char const * password, 
    char const * key, 
    MatchedRecord * outRecords, int maxRecords );

}

#endif  //  i_rosterquery_h
