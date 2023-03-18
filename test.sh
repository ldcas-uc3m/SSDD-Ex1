#!/bin/bash

# ---
# Runs the tests
# ---

cd src/

# compile
make servidor
make tests

# link dynamic library
export LD_LIBRARY_PATH=lib/

# run server
./servidor &

# run tests
./test_linked_list
./test_conc
./test_seq