import sys
import os
import glob
import subprocess

if __name__ == '__main__':
    ARTEMIS_WORK = os.environ['ARTEMIS_WORKDIR']

    # high si26
    output = ARTEMIS_WORK + "/output/coin/si26/high_si26_proton.root"

    inputs = ""
    input_array = glob.glob(ARTEMIS_WORK + "/output/coin/si26/proton/high/*.root");
    for i in range(len(input_array)):
        inputs += input_array[i] + " "

    cmd = "cd " + ARTEMIS_WORK + "; artemis -l '" + ARTEMIS_WORK + "/scripts/processline.C(\"mergefile " + output + " " + inputs + "\")'"

    print(cmd)
    subprocess.call([cmd], shell=True, executable="/usr/bin/zsh")

    # high al25
    output = ARTEMIS_WORK + "/output/coin/al25/high_al25_proton.root"

    inputs = ""
    input_array = glob.glob(ARTEMIS_WORK + "/output/coin/al25/proton/high/*.root");
    for i in range(len(input_array)):
        inputs += input_array[i] + " "

    cmd = "cd " + ARTEMIS_WORK + "; artemis -l '" + ARTEMIS_WORK + "/scripts/processline.C(\"mergefile " + output + " " + inputs + "\")'"

    print(cmd)
    subprocess.call([cmd], shell=True, executable="/usr/bin/zsh")

    # high mg24
    output = ARTEMIS_WORK + "/output/coin/mg24/high_mg24_proton.root"

    inputs = ""
    input_array = glob.glob(ARTEMIS_WORK + "/output/coin/mg24/proton/high/*.root");
    for i in range(len(input_array)):
        inputs += input_array[i] + " "

    cmd = "cd " + ARTEMIS_WORK + "; artemis -l '" + ARTEMIS_WORK + "/scripts/processline.C(\"mergefile " + output + " " + inputs + "\")'"

    print(cmd)
    subprocess.call([cmd], shell=True, executable="/usr/bin/zsh")

    # bg si26
    output = ARTEMIS_WORK + "/output/coin/si26/bg_si26_proton.root"

    inputs = ""
    input_array = glob.glob(ARTEMIS_WORK + "/output/coin/si26/proton/bg/*.root");
    for i in range(len(input_array)):
        inputs += input_array[i] + " "

    cmd = "cd " + ARTEMIS_WORK + "; artemis -l '" + ARTEMIS_WORK + "/scripts/processline.C(\"mergefile " + output + " " + inputs + "\")'"

    print(cmd)
    subprocess.call([cmd], shell=True, executable="/usr/bin/zsh")

    # bg al25
    output = ARTEMIS_WORK + "/output/coin/al25/bg_al25_proton.root"

    inputs = ""
    input_array = glob.glob(ARTEMIS_WORK + "/output/coin/al25/proton/bg/*.root");
    for i in range(len(input_array)):
        inputs += input_array[i] + " "

    cmd = "cd " + ARTEMIS_WORK + "; artemis -l '" + ARTEMIS_WORK + "/scripts/processline.C(\"mergefile " + output + " " + inputs + "\")'"

    print(cmd)
    subprocess.call([cmd], shell=True, executable="/usr/bin/zsh")

    # bg mg24
    output = ARTEMIS_WORK + "/output/coin/mg24/bg_mg24_proton.root"

    inputs = ""
    input_array = glob.glob(ARTEMIS_WORK + "/output/coin/mg24/proton/bg/*.root");
    for i in range(len(input_array)):
        inputs += input_array[i] + " "

    cmd = "cd " + ARTEMIS_WORK + "; artemis -l '" + ARTEMIS_WORK + "/scripts/processline.C(\"mergefile " + output + " " + inputs + "\")'"

    print(cmd)
    subprocess.call([cmd], shell=True, executable="/usr/bin/zsh")

