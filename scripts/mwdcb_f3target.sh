#!/usr/bin/zsh
echo "Start converting at $(date) with PID $$ "

artemis 'scripts/mwdcb_f3target.C()'

echo "End converting at $(date)"
