#!/bin/bash

# Start the server in the background
echo "[*] Starting server..."
python3 server.py &
SERVER_PID=$!

# Wait a brief moment to ensure server binds to port
sleep 1

# Run the client
echo "[*] Running client..."
python3 client.py

# Kill the server
echo "[*] Shutting down server..."
kill $SERVER_PID
echo "[*] Done."
