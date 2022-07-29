#! /bin/bash

[ $# -eq 1 ] && [ -d $1 ] || exit 1

find $1 -type l -exec test ! -e {} \; -printf '%p %y %Y \n' 2>/dev/null
