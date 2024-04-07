#!/bin/bash

# Check if a file path is provided as an argument
if [ "$#" -ne 1 ]; then
    echo "Usage: $0 /path/to/requirements.txt"
    exit 1
fi

# Store the requirements file path
REQUIREMENTS_FILE=$1

# Check if the requirements file exists
if [ ! -f "$REQUIREMENTS_FILE" ]; then
    echo "The specified requirements.txt file does not exist."
    exit 1
fi

# Delete the virtual environment if it exists
if [ -d "/tmp/.venv" ]; then
    echo "Deleting existing virtual environment at /tmp/.venv"
    rm -rf /tmp/.venv
fi

# Create a new virtual environment
echo "Creating a new virtual environment at /tmp/.venv"
python -m venv /tmp/.venv

# Activate the virtual environment
# Note: This part of sourcing in a script won't affect the parent shell.
source /tmp/.venv/bin/activate

# Install dependencies from the requirements file
pip install -r "$REQUIREMENTS_FILE"

echo "Dependencies installed successfully."
