#!/usr/bin/zsh

echo "Start analysis using 10 core! $$ "

taskset -c 0-15 ./scripts/check.sh 
