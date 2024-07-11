#!/usr/bin/zsh
prog=ReadElastic_RC.C
echo "Start converting at $(date) with PID $$ "

run181=146
#echo "End converting at $(date)"

for ((runnum = 145; runnum <run181; runnum++))
do
   root -l -q -b -e 'ReadElastic_RC.C("0145")'
   echo "Processing the run0$runnum.root"
done

echo "End converting at $(date)"
