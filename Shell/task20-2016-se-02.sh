#! /bin/bash

file="./file20"

echo "$(cat ${file} | cut -d "-" -f 2,3 | awk '{print NR "." $0}'  | sort -t "." -k 2)"
