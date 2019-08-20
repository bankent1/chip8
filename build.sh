#!/bin/bash

# exit on error
set -e

if [ ! -d obj ]; then
	mkdir obj
fi

# build the program
make all