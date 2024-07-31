#!/usr/bin/zsh

echo "Start analysis using 20 core! $$ "

taskset -c 0-19 ./scripts/mwdcb_f3target.sh 
