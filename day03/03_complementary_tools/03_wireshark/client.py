import socket
import time
from protocol import (
    HEADER_SIZE, MSG_TYPE_REQUEST, MSG_TYPE_RESPONSE, MAGIC_VALUE,
    create_packet, parse_header, pack_request_body, unpack_response_body
)

PORT = 9999
HOST = "127.0.0.1"
CLIENT_ID = 1001
PROTOCOL_VERSION = 5

def connect_and_send(messages):
    client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    try:
        client.connect((HOST, PORT))
        print(f"[*] Connected to {HOST}:{PORT} as Client {CLIENT_ID}")
        
        for msg in messages:
            print(f"[*] Sending REQUEST: {msg}")
            # Format custom body
            request_payload = pack_request_body(CLIENT_ID, PROTOCOL_VERSION, msg)
            packet = create_packet(MSG_TYPE_REQUEST, request_payload)
            client.sendall(packet)
            
            # Read response header
            header_data = client.recv(HEADER_SIZE)
            if len(header_data) == HEADER_SIZE:
                magic, msg_type, reserved, payload_len = parse_header(header_data)
                
                if magic == MAGIC_VALUE and msg_type == MSG_TYPE_RESPONSE:
                    # Read response payload
                    payload_data = b""
                    while len(payload_data) < payload_len:
                        chunk = client.recv(payload_len - len(payload_data))
                        if not chunk:
                            break
                        payload_data += chunk
                        
                    # Parse custom body
                    status_code, response_msg = unpack_response_body(payload_data)
                    print(f"[*] Received RESPONSE [{status_code} OK]: {response_msg}")
                else:
                    print(f"[-] Invalid response header: type={msg_type}")
            else:
                print("[-] Failed to read full header from server")
            
            time.sleep(1) # Pause between messages
            print("-" * 40)
            
    except ConnectionRefusedError:
        print(f"[-] Connection refused. Is the server running on {HOST}:{PORT}?")
    except Exception as e:
        print(f"[-] Error: {e}")
    finally:
        client.close()
        print("[*] Connection closed")

if __name__ == "__main__":
    messages_to_send = [
        "First advanced message!",
        "Simulating complex protocol logic.",
        "Disconnecting."
    ]
    connect_and_send(messages_to_send)
