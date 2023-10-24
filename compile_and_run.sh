#!/bin/bash
# This script compiles the CPS programs and starts the servers and a client instance in separate xterm windows.
# Author: Paul Roode

gcc cps_main.c child_process.c -o cps_main
gcc ecg_server.c server_process.c -o ecg_server
gcc image_server.c server_process.c -o image_server

echo "Compilation completed."

# Start the ECG and image servers in new xterm windows
xterm -e ./ecg_server &
xterm -e ./image_server &

# Delay to give servers a moment to start
sleep 2

# Start the main CPS program in a new xterm window
xterm -hold -e ./cps_main &

echo "You can now run additional instances of the main CPS program if desired."
