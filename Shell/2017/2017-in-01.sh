#! /bin/bash

find / -printf '%U\n' 2>/dev/null | egrep "^$(id -u)$" | wc -l
