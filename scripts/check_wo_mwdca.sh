#!/usr/bin/zsh
echo "Start converting at $(date) with PID $$ "

artemis 'scripts/proton_wo_mwdca.C("run", "0099", "tel1")'
artemis 'scripts/proton_wo_mwdca.C("run", "0099", "tel2")'
artemis 'scripts/proton_wo_mwdca.C("run", "0099", "tel3")'
artemis 'scripts/proton_wo_mwdca.C("run", "0099", "tel4")'
artemis 'scripts/proton_wo_mwdca.C("run", "0099", "tel5")'
artemis 'scripts/proton_wo_mwdca.C("run", "0099", "tel6")'

echo "End converting at $(date)"
