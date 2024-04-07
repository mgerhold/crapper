#!/bin/bash

# Check if two arguments are provided
if [ "$#" -ne 2 ]; then
    echo "Usage: $0 /path/to/http-server/main.py <socket-port>"
    exit 1
fi

# Store the arguments in variables
PYTHON_SCRIPT=$1
SOCKET_PORT=$2

# Source the activate file to use the virtual environment
source /tmp/.venv/bin/activate

# Run the Python script with the socket port argument
python "$PYTHON_SCRIPT" --socket-port "$SOCKET_PORT"

# Deactivate the virtual environment
deactivate
