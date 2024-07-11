import os
import subprocess

if __name__ == '__main__':
    ARTEMIS_WORK = os.environ['ARTEMIS_WORKDIR']

    names = ["high", "bg"]
    gates = ["si26", "al25", "mg24"]
    tels = ["tel1", "tel2", "tel3", "tel4", "tel5"]

    # si26
    procs = []
    for i in range(len(tels)):
        cmd = "cd " + ARTEMIS_WORK + "; artemis -l '" + ARTEMIS_WORK + "/scripts/proton.C(\"high\",\"0000\",\"si26\",\"" + tels[i] + "\")'"
        print(cmd)
        procs.append(subprocess.Popen([cmd], shell=True, executable="/usr/bin/zsh"))

    for i in range(len(tels)):
        cmd = "cd " + ARTEMIS_WORK + "; artemis -l '" + ARTEMIS_WORK + "/scripts/proton.C(\"bg\",\"0000\",\"si26\",\"" + tels[i] + "\")'"
        print(cmd)
        procs.append(subprocess.Popen([cmd], shell=True, executable="/usr/bin/zsh"))

    for proc in procs:
        proc.communicate()

    # al25
    procs = []
    for i in range(len(tels)):
        cmd = "cd " + ARTEMIS_WORK + "; artemis -l '" + ARTEMIS_WORK + "/scripts/proton.C(\"high\",\"0000\",\"al25\",\"" + tels[i] + "\")'"
        print(cmd)
        procs.append(subprocess.Popen([cmd], shell=True, executable="/usr/bin/zsh"))

    for i in range(len(tels)):
        cmd = "cd " + ARTEMIS_WORK + "; artemis -l '" + ARTEMIS_WORK + "/scripts/proton.C(\"bg\",\"0000\",\"al25\",\"" + tels[i] + "\")'"
        print(cmd)
        procs.append(subprocess.Popen([cmd], shell=True, executable="/usr/bin/zsh"))

    for proc in procs:
        proc.communicate()

    # mg24
    procs = []
    for i in range(len(tels)):
        cmd = "cd " + ARTEMIS_WORK + "; artemis -l '" + ARTEMIS_WORK + "/scripts/proton.C(\"high\",\"0000\",\"mg24\",\"" + tels[i] + "\")'"
        print(cmd)
        procs.append(subprocess.Popen([cmd], shell=True, executable="/usr/bin/zsh"))

    for i in range(len(tels)):
        cmd = "cd " + ARTEMIS_WORK + "; artemis -l '" + ARTEMIS_WORK + "/scripts/proton.C(\"bg\",\"0000\",\"mg24\",\"" + tels[i] + "\")'"
        print(cmd)
        procs.append(subprocess.Popen([cmd], shell=True, executable="/usr/bin/zsh"))

    for proc in procs:
        proc.communicate()

