#!/usr/local/bin/zsh

arthome=$(cd $(dirname $0);pwd)
time=$(date)

function usage() {
   echo "usage: ./setmuxprm.sh \$1"
   echo "usage: ex) ./setmuxprm.sh physics0100"
}

if [ $1_ = _ ] ; then
   usage $0
   return
fi

if [ $# -ne 1 ];then
   usage $0
   return
fi

# need this format!
mux11_usage="tel6:dEX"
mux12_usage="tel6:dEY"
mux21_usage="tel5:dEX"
mux22_usage="tel5:dEY"
mux31_usage="tel4:dEX"
mux32_usage="tel4:dEY"

mux11_array=(${=mux11_usage//:/ })
mux12_array=(${=mux12_usage//:/ })
mux21_array=(${=mux21_usage//:/ })
mux22_array=(${=mux22_usage//:/ })
mux31_array=(${=mux31_usage//:/ })
mux32_array=(${=mux32_usage//:/ })


#===== MUX parameters =====
mux11_prm_dir="$arthome/prm/ssd/${mux11_array[1]}/pos_${mux11_array[2]}"
cd $mux11_prm_dir
if [ -f "$1.dat" ] ; then
   echo "MUX1-1 $mux11_usage position prm: $mux11_prm_dir/$1 is current"
   rm -f current
   ln -sf "$1.dat" current
else
   echo "ERROR: $mux11_prm_dir/$1 not found."
   return
fi


mux12_prm_dir="$arthome/prm/ssd/${mux12_array[1]}/pos_${mux12_array[2]}"
cd $mux12_prm_dir
if [ -f "$1.dat" ] ; then
   echo "MUX1-2 $mux12_usage position prm: $mux12_prm_dir/$1 is current"
   rm -f current
   ln -sf "$1.dat" current
else
   echo "ERROR: $mux12_prm_dir/$1 not found."
   return
fi


mux21_prm_dir="$arthome/prm/ssd/${mux21_array[1]}/pos_${mux21_array[2]}"
cd $mux21_prm_dir
if [ -f "$1.dat" ] ; then
   echo "MUX2-1 $mux21_usage position prm: $mux21_prm_dir/$1 is current"
   rm -f current
   ln -sf "$1.dat" current
else
   echo "ERROR: $mux21_prm_dir/$1 not found."
   return
fi


mux22_prm_dir="$arthome/prm/ssd/${mux22_array[1]}/pos_${mux22_array[2]}"
cd $mux22_prm_dir
if [ -f "$1.dat" ] ; then
   echo "MUX2-2 $mux22_usage position prm: $mux22_prm_dir/$1 is current"
   rm -f current
   ln -sf "$1.dat" current
else
   echo "ERROR: $mux22_prm_dir/$1 not found."
   return
fi


mux31_prm_dir="$arthome/prm/ssd/${mux31_array[1]}/pos_${mux31_array[2]}"
cd $mux31_prm_dir
if [ -f "$1.dat" ] ; then
   echo "MUX3-1 $mux31_usage position prm: $mux31_prm_dir/$1 is current"
   rm -f current
   ln -sf "$1.dat" current
else
   echo "ERROR: $mux31_prm_dir/$1 not found."
   return
fi


mux32_prm_dir="$arthome/prm/ssd/${mux32_array[1]}/pos_${mux32_array[2]}"
cd $mux32_prm_dir
if [ -f "$1.dat" ] ; then
   echo "MUX3-2 $mux32_usage position prm: $mux32_prm_dir/$1 is current"
   rm -f current
   ln -sf "$1.dat" current
else
   echo "ERROR: $mux32_prm_dir/$1 not found."
   return
fi


cd $arthome
echo "${time} using $1 prm" >> $arthome/.log_mux
