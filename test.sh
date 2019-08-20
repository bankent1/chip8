#!/bin/bash

if [ ! -d obj ]; then
	mkdir obj
fi

make runtests

echo "====================="
echo "    RUNNING TESTS"
echo "====================="
./runtests