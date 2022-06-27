#! /bin/bash

find . -type f -printf '%n %p\n' | sort -n -k 1,1 | tail -5 | cut -d " " -f 2-
