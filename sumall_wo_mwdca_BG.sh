#!/usr/bin/zsh
# physics run
tel1=/home/zq/art_analysis/he6p2024/he6p2024/output/coin/gate/phyrun/proton/tel1/run0141_proton.hist_mwdca.root
tel2=/home/zq/art_analysis/he6p2024/he6p2024/output/coin/gate/phyrun/proton/tel2/run0141_proton.hist_mwdca.root
tel3=/home/zq/art_analysis/he6p2024/he6p2024/output/coin/gate/phyrun/proton/tel3/run0141_proton.hist_mwdca.root
tel4=/home/zq/art_analysis/he6p2024/he6p2024/output/coin/gate/phyrun/proton/tel4/run0141_proton.hist_mwdca.root
tel5=/home/zq/art_analysis/he6p2024/he6p2024/output/coin/gate/phyrun/proton/tel5/run0141_proton.hist_mwdca.root
tel6=/home/zq/art_analysis/he6p2024/he6p2024/output/coin/gate/phyrun/proton/tel6/run0141_proton.hist_mwdca.root

hadd elastic_phy_141_wo_mwdca.root $tel1 $tel2 $tel3 $tel4 $tel5 $tel6
