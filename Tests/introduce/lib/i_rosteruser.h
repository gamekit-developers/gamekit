
#if !defined( i_rosteruser_h )
#define i_rosteruser_h

#include "roster.h"

namespace libintroduce {

struct MatchedRecord;
class I_Roster;

class I_RosterUser {
public:
  //  disposing() is called back when the roster is going away
  virtual void disposing( I_Roster * roster ) = 0;
  //  validatePeer() is called when a user attempts to connect; return TRUE if you 
  //  want to allow the user to proceed. How you store user information and passwords 
  //  is your problem. The user name and password fields can each be up to 64 chars.
  virtual bool validatePeer( I_Roster * roster, sockaddr_in const & peer, 
      char const * name, char const * password ) = 0;
  //  userTimeout() is called when a user is timed out because of inactivity.
  virtual void userTimeout( I_Roster * roster, sockaddr_in const & peer ) = 0;
  //  registerRecord() is called when a user attempts to register a record; call 
  //  back into the roster to register the record if you agree.
  //  The token is supplied by the peer, and used by the peer to identify the 
  //  registration (to avoid duplicates in case of network problems).
  virtual void registerRecord( I_Roster * roster, sockaddr_in const & peer, 
      char const * record, int token ) = 0;
  //  matchRecords() is called when a user attempts to query records; you can 
  //  call back into the roster to match records if you agree. Return the number 
  //  of actual records matched.
  virtual int matchRecords( I_Roster * roster, sockaddr_in const & peer, 
      char const * key, int id, MatchedRecord * outMatches, int maxMatches ) = 0;
  //  allocIntroduction() is called when a user attempts to get introduced to 
  //  another peer. Typically, you will call back into the roster to make a 
  //  introduce() call.
  virtual void allowIntroduction( I_Roster * roster, sockaddr_in const & peer1, 
      sockaddr_in const & peer2 ) = 0;
};

}

#endif  //  i_rosteruser_h

