#!/bin/bash

if [ ! -d obj ]; then
	mkdir obj
fi

make tests

echo "====================="
echo "    RUNNING TESTS"
echo "====================="
./tests