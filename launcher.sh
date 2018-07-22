#!/bin/bash
echo "antes"
export LD_LIBRARY_PATH="/usr/local/lib"
./spotathome configuration_file.json
echo "despues"
