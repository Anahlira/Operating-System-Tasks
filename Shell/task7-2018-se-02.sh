#! /bin/bash

find ~pesho -links +2 -printf '%T@ %i\n' | sort -n -k 1,1 | tail -1 | cut -d " " -f 2
