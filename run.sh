#!/bin/bash

# exit on error
set -e

./build.sh

# run the program
echo "============================="
echo "Running CHIP 8 Emulator"
echo "============================="
./chip8