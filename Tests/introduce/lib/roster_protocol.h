
#if !defined( roster_protocol_h )
#define roster_protocol_h

#include "message.h"

namespace libintroduce {

//! All packets in my protocol start with \c ProtoHeader.
//! \note I haven't taken the trouble to make this protocol endian 
//! safe. Use it only on machines all of the same kind.
//! To make determining this a little better (and make it possible 
//! to wire them up in the future), the "type" field should be 
//! or-ed with \e TypeBigEndian if sent from a big-endian machine. 
//! The \c C_Roster class has a static named \m bigEndian_ which 
//! I can bit-or in for this particular purpose.
//! Note that making "size" be an unsigned byte restricts the maximum 
//! message size, compared to what the underlying transport can deal 
//! with. On a modem, 256 bytes takes 100 milliseconds to send, so 
//! it's probably a good idea to keep this restriction.

struct ProtoHeader {
  unsigned char type;
  unsigned char size;
  //! When creating a message for sending, you can first create the 
  //! entire message (minus header) and then prepend a protoheader 
  //! using ProtoHeader::prepend( Type, theMessage ); this is very 
  //! convenient.
  static bool prepend( unsigned char t, Message & m ) {
    ProtoHeader ph;
    ph.type = t;
    ph.size = m.size_;
    return m.prepend( ph );
  }
};

//! The "type" field of \c ProtoHeader can take the following values.
enum ProtoHeader_Type {
  Keepalive = 0,        //!<  client->server
  Connect = 1,          //!<  client->server
  ConnectAck = 2,       //!<  server->client
  Disconnect = 3,       //!<  client->server (not acked)
  Record = 4,           //!<  client->server
  RecordAck = 5,        //!<  server->client
  Query = 6,            //!<  client->server (answer is ack)
  Answer = 7,           //!<  server->client
  Kick = 8,             //!<  server->client
  IntroducePeer = 9,    //!<  client->server
  IntroduceAck = 10,    //!<  server->client

  User = 32,            //!<  user types can start numbering here

  TypeBigEndian = 0x80, //!<  OR this in on a big-endian host
};

}

#endif  //  roster_protocol_h
