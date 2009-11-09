
#if !defined( roster_h )
#define roster_h

#include "network.h"
#include "clock.h"
#include "i_rosteruser.h"
#include "i_rosterquery.h"
#include "i_rosterqueryuser.h"
#include "i_rosterconnectionnotify.h"

namespace libintroduce {

//  The Roster allows a host to export a network introduction service, 
//  and allows a client to find matching peers. The way this works is 
//  that the roster keeps a list of <public IP> : <information record> 
//  tuples. A client can connect, giving a name and password, which the 
//  roster user can reject. Once accepted, the client can issue 
//  queries that can be matched up to the information record. The 
//  roster user is in control over exactly how the matching is done, 
//  although the Roster has a default implementation that most users 
//  will take advantage of.

//  For the client side interface, look at I_RosterQuery.

class I_Socket;
class I_RosterUser;
class I_Roster;

//  Anyone wanting to host a roster can create a I_Roster, either having the 
//  Roster create the socket, or by borrowing an existing socket.
//  If you create me with a network and port, I will create my own socket, 
//  and I will dispose it when I'm disposed.
I_Roster * CreateRoster( I_Clock * clock, I_Network * network, unsigned short port, I_RosterUser * user );
//  If you create me with a socket, I will not dispose that socket when disposed.
I_Roster * CreateRoster( I_Clock * clock, I_Socket * socket, I_RosterUser * user );


//! The lifetime for \m record is only until you next call back into 
//! the roster, or into a system that may deliver messages to the 
//! roster.
struct MatchedRecord {
  sockaddr_in addr;
  char const * record;    //!< copy this as soon as the value is returned
  float score;            //!< between 0 and 1
};

class I_Roster {
public:
  //! \f dispose() me when you're done
  virtual void dispose() = 0;
  //  accessors for configured properties
  virtual I_Clock * clock() = 0;
  virtual I_Network * network() = 0;
  virtual I_Socket * socket() = 0;
  virtual I_RosterUser * rosterUser() = 0;

  //! \f step() me every so often, so that I can expire old peers.
  virtual void step() = 0;
  //! When you hear about a client that wants to register a record, call me back to 
  //! manage that record. A peer can have multiple records. \returns false if the 
  //! implementation-defined limit to record storage would be exceeded.
  //! The token is passed in by the peer, and used by him/her to identify this 
  //! particular record (to avoid duplication).
  virtual bool addRecord( sockaddr_in const & peer, char const * record, int token ) = 0;
  //! \f removeRecords() removes all records associated with a peer address.
  virtual void removeRecords( sockaddr_in const & peer ) = 0;
  //! Match and return multiple records. Returns how many records were actually returned. 
  //! All returned records will have a score >= threshold. At most maxMatches records 
  //! will be returned.
  virtual int matchRecords( char const * key, MatchedRecord * outMatches, 
      int maxMatches, float threshold ) = 0;
  //! Send out introduction packets to both peers, to let them talk to each other.
  virtual void introduce( sockaddr_in const & peer1, sockaddr_in const & peer2 ) = 0;
};

//! The format of a record is a list of semicolon separated variable=value values.
//! The format if a key is a list of semicolon separated variable<op>value relations.
//! MatchRecord will return 1.0 if the key matches perfectly, and 0.0 if the key 
//! matches not at all, compared to the values in the record. "%" matches any string.
//! It is impossible to register or match a semicolon for any value.
//! Example:
//! record: "score=1300;location=us.ca;length=20"
//! key:    "score>1000;score<1200;location=us.%;length=20;importance=high"
//! return value from MatchRecord(record,key) is 0.6f; "score<1200" is not true, 
//! and "importance=high" is not true (query attributes missing in the record are 
//! always assumed not matching).
//! For strings, '=' and '=' with wildcard (%) are the only allowed operations.
//! For integers, '<', '=' and '>' are the allowed operations.
//! String values cannot start with a digit.
//! There is a 63-character limit on each individual variable name length, and a 
//! 255-character limit on each variable value length. There is a ~1300 byte limit 
//! on the total length of a record. Note that the underlying protocol restricts 
//! these numbers further; down to about 230 characters per record at the most.
//! UTF-8 strings will work with this function.
//! The record MUST BE tightly spaced; whitespace is significant!
//! "var=value;var2=value2" is very different from "var = value; var2=value2".
float MatchRecord( char const * record, char const * key );

}

#include "roster_protocol.h"

#endif  //  roster_h
