import socket
import struct
import threading
from protocol import (
    HEADER_SIZE, MSG_TYPE_REQUEST, MSG_TYPE_RESPONSE, MAGIC_VALUE,
    create_packet, parse_header, unpack_request_body, pack_response_body
)

PORT = 9999
HOST = "127.0.0.1"

def handle_client(conn, addr):
    print(f"[+] New connection from {addr}")
    try:
        while True:
            # 1. Read Header
            header_data = conn.recv(HEADER_SIZE)
            if not header_data:
                break
            if len(header_data) < HEADER_SIZE:
                print(f"[-] Invalid header size from {addr}")
                break
                
            magic, msg_type, reserved, payload_len = parse_header(header_data)
            
            if magic != MAGIC_VALUE:
                print(f"[-] Invalid magic bytes from {addr}")
                break
                
            # 2. Read Payload
            payload_data = b""
            while len(payload_data) < payload_len:
                chunk = conn.recv(payload_len - len(payload_data))
                if not chunk:
                    break
                payload_data += chunk
                
            if len(payload_data) < payload_len:
                print(f"[-] Incomplete payload from {addr}")
                break
                
            # 3. Process Request
            if msg_type == MSG_TYPE_REQUEST:
                client_id, version, msg_str = unpack_request_body(payload_data)
                print(f"[*] Received REQUEST from Client {client_id} (v{version}): {msg_str}")
                
                # Send Response
                response_str = f"Server processed: {msg_str}"
                response_payload = pack_response_body(200, response_str) # 200 OK
                response_packet = create_packet(MSG_TYPE_RESPONSE, response_payload)
                
                conn.sendall(response_packet)
                print(f"[*] Sent RESPONSE to Client {client_id}")
            else:
                print(f"[-] Unexpected message type {msg_type} from {addr}")
                
    except Exception as e:
        print(f"[-] Error handling {addr}: {e}")
    finally:
        conn.close()
        print(f"[-] Connection {addr} closed")

def start_server():
    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server.bind((HOST, PORT))
    server.listen(5)
    print(f"[*] Server listening on {HOST}:{PORT}")
    
    try:
        while True:
            conn, addr = server.accept()
            thread = threading.Thread(target=handle_client, args=(conn, addr))
            thread.daemon = True
            thread.start()
    except KeyboardInterrupt:
        print("\n[*] Server shutting down")
    finally:
        server.close()

if __name__ == "__main__":
    start_server()
