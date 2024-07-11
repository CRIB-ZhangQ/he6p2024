#!/bin/sh

if [ $# -ne 2 ];
then 
  exit 1
fi

grep -rnw "$1" -e "$2"
