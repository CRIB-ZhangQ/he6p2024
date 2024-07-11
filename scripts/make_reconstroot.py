import os
import subprocess

if __name__ == '__main__':
    ARTEMIS_WORK = os.environ['ARTEMIS_WORKDIR']

    names = ["high", "bg"]
    gates = ["si26", "al25", "mg24"]

    procs = []
    for i in range(len(names)):
        for j in range(len(gates)):
            cmd = "cd " + ARTEMIS_WORK + "; artemis -l '" + ARTEMIS_WORK + "/scripts/reconst.C(\"" + names[i] + "\",\"0000\",\"" + gates[j] + "\")'"
            print(cmd)
            procs.append(subprocess.Popen([cmd], shell=True, executable="/usr/bin/zsh"))

    for proc in procs:
        proc.communicate()


