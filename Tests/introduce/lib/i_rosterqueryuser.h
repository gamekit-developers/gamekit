
#if !defined( i_rosterqueryuser_h )
#define i_rosterqueryuser_h

#include "roster.h"

namespace libintroduce {

struct MatchedRecord;
class I_RosterQuery;

class I_RosterQueryUser {
public:
  //  When the query connection is going away, I'll be told, if 
  //  I still have a query outstanding.
  virtual void disposing( I_RosterQuery * query ) = 0;
  //  when the query completes or fails, I'm told about the 
  //  results, or the failure. The query will then forget about 
  //  me -- I will not get disposing() at that point.
  virtual void onQueryResult( I_RosterQuery * query, 
      MatchedRecord const * matched, int numMatched ) = 0;
  virtual void onQueryTimeout( I_RosterQuery * query ) = 0;
};

}

#endif  //  i_rosterqueryuser_h

