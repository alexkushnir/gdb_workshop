import struct

# Protocol Header Format (8 bytes total)
# 2 bytes: Magic ('G', 'D')
# 1 byte: Message Type (1 = Request, 2 = Response)
# 1 byte: Reserved (0x00)
# 4 bytes: Payload Length (Unsigned Int, Big Endian)
HEADER_FORMAT = "!2s B B I"
HEADER_SIZE = struct.calcsize(HEADER_FORMAT)
MAGIC_VALUE = b"GD"

MSG_TYPE_REQUEST = 1
MSG_TYPE_RESPONSE = 2

# Request Body:
# 4 bytes: Client ID (Unsigned Int)
# 2 bytes: Protocol Version (Unsigned Short)
# Variable length string: Message (derived from total Payload Length - 6)
REQUEST_BODY_FMT_BASE = "!I H"
REQUEST_BODY_BASE_SIZE = struct.calcsize(REQUEST_BODY_FMT_BASE)

# Response Body:
# 2 bytes: Status Code (Unsigned Short, e.g., 200 for OK)
# Variable length string: Message (derived from total Payload Length - 2)
RESPONSE_BODY_FMT_BASE = "!H"
RESPONSE_BODY_BASE_SIZE = struct.calcsize(RESPONSE_BODY_FMT_BASE)

def create_packet(msg_type, payload: bytes) -> bytes:
    """Create a complete packet with header and payload."""
    payload_len = len(payload)
    header = struct.pack(HEADER_FORMAT, MAGIC_VALUE, msg_type, 0, payload_len)
    return header + payload

def parse_header(header_bytes: bytes):
    """Parse header and return (magic, msg_type, reserved, payload_len)."""
    if len(header_bytes) < HEADER_SIZE:
        raise ValueError("Header too short")
    return struct.unpack(HEADER_FORMAT, header_bytes)

def pack_request_body(client_id: int, version: int, msg: str) -> bytes:
    """Pack the structured request body."""
    msg_bytes = msg.encode('utf-8')
    base_body = struct.pack(REQUEST_BODY_FMT_BASE, client_id, version)
    return base_body + msg_bytes

def unpack_request_body(payload: bytes):
    """Unpack a structured request body and return (client_id, version, msg)."""
    if len(payload) < REQUEST_BODY_BASE_SIZE:
        raise ValueError("Request body too short")
    client_id, version = struct.unpack(REQUEST_BODY_FMT_BASE, payload[:REQUEST_BODY_BASE_SIZE])
    msg = payload[REQUEST_BODY_BASE_SIZE:].decode('utf-8')
    return client_id, version, msg

def pack_response_body(status_code: int, msg: str) -> bytes:
    """Pack the structured response body."""
    msg_bytes = msg.encode('utf-8')
    base_body = struct.pack(RESPONSE_BODY_FMT_BASE, status_code)
    return base_body + msg_bytes

def unpack_response_body(payload: bytes):
    """Unpack a structured response body and return (status_code, msg)."""
    if len(payload) < RESPONSE_BODY_BASE_SIZE:
        raise ValueError("Response body too short")
    status_code = struct.unpack(RESPONSE_BODY_FMT_BASE, payload[:RESPONSE_BODY_BASE_SIZE])[0]
    msg = payload[RESPONSE_BODY_BASE_SIZE:].decode('utf-8')
    return status_code, msg
