#!/usr/bin/zsh
echo "Start converting at $(date) with PID $$ "

artemis 'scripts/proton_bg_wo_mwdca.C("run", "0141", "tel1")'
artemis 'scripts/proton_bg_wo_mwdca.C("run", "0141", "tel2")'
artemis 'scripts/proton_bg_wo_mwdca.C("run", "0141", "tel3")'
artemis 'scripts/proton_bg_wo_mwdca.C("run", "0141", "tel4")'
artemis 'scripts/proton_bg_wo_mwdca.C("run", "0141", "tel5")'
artemis 'scripts/proton_bg_wo_mwdca.C("run", "0141", "tel6")'

echo "End converting at $(date)"
