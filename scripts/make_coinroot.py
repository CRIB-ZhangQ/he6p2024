import sys
import os
import subprocess

if __name__ == '__main__':
    ARTEMIS_WORK = os.environ['ARTEMIS_WORKDIR']
    RUNINFO = sys.argv[1]

    RUNNAME = RUNINFO[:-4]
    RUNNUM  = RUNINFO[-4:]

    if int(RUNNUM) < 249:
        cmd = "cd " + ARTEMIS_WORK + "; " + ARTEMIS_WORK + "/build/run_artemis " + ARTEMIS_WORK + "/steering/chkbeamcoin_before.yaml " + RUNNAME + " " + RUNNUM + " si26"
        print(cmd)
        subprocess.call([cmd], shell=True, executable="/usr/bin/zsh")

        cmd = "cd " + ARTEMIS_WORK + "; " + ARTEMIS_WORK + "/build/run_artemis " + ARTEMIS_WORK + "/steering/chkbeamcoin_before.yaml " + RUNNAME + " " + RUNNUM + " al25"
        print(cmd)
        subprocess.call([cmd], shell=True, executable="/usr/bin/zsh")

        cmd = "cd " + ARTEMIS_WORK + "; " + ARTEMIS_WORK + "/build/run_artemis " + ARTEMIS_WORK + "/steering/chkbeamcoin_before.yaml " + RUNNAME + " " + RUNNUM + " mg24"
        print(cmd)
        subprocess.call([cmd], shell=True, executable="/usr/bin/zsh")

    else:
        cmd = "cd " + ARTEMIS_WORK + "; " + ARTEMIS_WORK + "/build/run_artemis " + ARTEMIS_WORK + "/steering/chkbeamcoin_after.yaml " + RUNNAME + " " + RUNNUM + " si26"
        print(cmd)
        subprocess.call([cmd], shell=True, executable="/usr/bin/zsh")

        cmd = "cd " + ARTEMIS_WORK + "; " + ARTEMIS_WORK + "/build/run_artemis " + ARTEMIS_WORK + "/steering/chkbeamcoin_after.yaml " + RUNNAME + " " + RUNNUM + " al25"
        print(cmd)
        subprocess.call([cmd], shell=True, executable="/usr/bin/zsh")

        cmd = "cd " + ARTEMIS_WORK + "; " + ARTEMIS_WORK + "/build/run_artemis " + ARTEMIS_WORK + "/steering/chkbeamcoin_after.yaml " + RUNNAME + " " + RUNNUM + " mg24"
        print(cmd)
        subprocess.call([cmd], shell=True, executable="/usr/bin/zsh")


