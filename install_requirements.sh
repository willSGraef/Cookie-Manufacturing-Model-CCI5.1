#!/usr/bin/env bash

set -euo pipefail

# Resolve the script's directory to handle execution from any location
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SERVICE_HOME_DIR="${SCRIPT_DIR}/Simulation"

# Verify Simulation directory exists
if [[ ! -d "$SERVICE_HOME_DIR" ]]; then
    echo "Error: Simulation directory not found at $SERVICE_HOME_DIR" >&2
    exit 1
fi

# Check if Python3 is installed
if ! command -v python3 &> /dev/null; then
    echo "Python3 could not be found. Installing Python3..."
    sudo apt-get update
    sudo apt-get install -y python3.13 python3-venv python3-pip
else
    echo "Python3 is already installed. Moving on..."
fi

# Create virtual environment if it doesn't exist
VENV_DIR="${SERVICE_HOME_DIR}/.venv"
if [[ ! -d "$VENV_DIR" ]]; then
    echo "Creating a virtual environment and installing dependencies..."
    python3 -m venv "$VENV_DIR"
    source "${VENV_DIR}/bin/activate"
    pip install --upgrade pip
    pip install pyModbusTCP
    echo "All dependencies have been installed."
else
    echo "Virtual environment already exists. Skipping creation..."
    source "${VENV_DIR}/bin/activate"
fi

# Test the installation by running the main application
echo "Testing the installation by running the main application..."
cd "$SERVICE_HOME_DIR"

python3 main.py &
APP_PID=$!
sleep 2

# Check if the process is still running
if kill -0 "$APP_PID" 2>/dev/null; then
    echo "Installation and test run completed successfully."
    kill "$APP_PID" 2>/dev/null || true
    wait "$APP_PID" 2>/dev/null || true
else
    echo "Error: Failed to start main.py application." >&2
    exit 1
fi

# create service file to start on boot
SERVICE_FILE="/etc/systemd/system/simulation.service"
if [[ ! -f "$SERVICE_FILE" ]]; then
    echo "Creating systemd service file for Simulation..."
    sudo bash -c "cat > $SERVICE_FILE" <<EOL
[Unit]
Description=Simulation Service
After=network.target
[Service]
User=$USER
WorkingDirectory=$SERVICE_HOME_DIR
ExecStart=$VENV_DIR/bin/python3 $SERVICE_HOME_DIR/main.py
Restart=always
[Install]
WantedBy=multi-user.target
EOL

    echo "Enabling and starting the Simulation service..."
    sudo systemctl enable simulation.service
    sudo systemctl start simulation.service
fi
echo "Setup complete."
