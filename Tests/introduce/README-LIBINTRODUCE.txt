
LIBINTRODUCE
Copyright 2004 Jon Watte
2004-11-11  version 0.2


Libintroduce provides several gaming-related networking services:

- simple UDP sockets Windows/UNIX portability layer
- C++ interface based API to UDP sockets and select()
- Binary data marshaling and demarshaling and message formatting
- Matchmaking/lobbying server library
- Peer-to-peer NAT punch-through capability

For more information about the latter part, see my web page at 
http://www.mindcontrol.org/~hplus/nat-punch.html -- it has links 
to several other good references.

There is currently no manual, although there are functional/unit 
tests for most of the functionality. There are a few weak spots 
at the top of the stack, with regards to the end-to-end matchmaking, 
introduction, and game communication solution. This is work in 
progress.

LIBINTRODUCE is released under an MIT-style license. You don't have 
to pay anyone to use it commercially, and it doesn't "taint" your 
source with any further opensource requirements. However, you have 
to accept all liability for your use of (or inability to use) the 
source code -- Jon Watte must be held harmless against any claims 
arising out of your such use. The source is provided free of charge 
with no obligation, warrantee or guarantee.


The actual source is found in "lib". Add the .cpp files to your 
project or make file, and point your include path to the headers 
in that library. Start looking at "network.h" for the basic 
networking abstraction. Look at the test runner "app" or at the 
"getter," "putter" and "roster" applications for usage examples.
