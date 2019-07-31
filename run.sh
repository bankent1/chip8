#!/bin/bash

# exit on error
set -e

if [ ! -d target ]; then
	mkdir target
fi

# build the program
make

# run the program
echo "============================="
echo "Running CHIP 8 Emulator"
echo "============================="
./target/chip8
