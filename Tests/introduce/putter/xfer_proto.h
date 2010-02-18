
#if !defined( xfer_proto_h )
#define xfer_proto_h

#include "roster_protocol.h"

//  extends ProtoHeader_Type
enum Xfer_Type {
  XferReqeust = User,
  XferRequestAck,
  XferPacketRequest,
  XferPacket,
  XferDisconnect,
};

//  XferRequest
//  id (client's id, int)
//  name (cstring)
//  bytesPerSecond (int) -- client's request

//  XferRequestAck
//  id (client's id, int) (<=0 for error)
//  name/error (cstring)
//  numPackets (int) (if not error)
//  sizeofLastPacket (int) (all packets except last contain 1024 bytes)

//  XferPacketRequest
//  id (client's id, int)
//  packet (int)
//  numPackets (int)

//  XferPacket
//  id (client's id, int) (<= 0 for error)
//  packet (int)
//  <data>

//  XferDisconnect  -- advisory
//  <no data>

#endif  //  xfer_proto_h

