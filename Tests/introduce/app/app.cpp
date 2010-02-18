
#include "network.h"

extern void Unittest_Network();
extern void Unittest_Socket();
extern void Unittest_SocketPeer();
extern void Unittest_Message();
extern void Unittest_MatchRecord();
extern void Unittest_Roster();
extern void Unittest_RosterQuery();
extern void Unittest_Clock();

int
main()
{
  Unittest_Network();
  Unittest_Socket();
  Unittest_SocketPeer();
  Unittest_Message();
  Unittest_MatchRecord();
  Unittest_Roster();
  Unittest_RosterQuery();
  Unittest_Clock();
  return 0;
}
