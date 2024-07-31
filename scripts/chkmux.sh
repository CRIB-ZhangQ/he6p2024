#!/usr/bin/zsh
echo "Start converting at $(date) with PID $$ "

artemis -l 'macro/run_MUXParamMaker.C("run", "0072")'
artemis -l 'macro/run_MUXParamMaker.C("run", "0073")'
artemis -l 'macro/run_MUXParamMaker.C("run", "0078")'
artemis -l 'macro/run_MUXParamMaker.C("run", "0079")'
artemis -l 'macro/run_MUXParamMaker.C("run", "0080")'
artemis -l 'macro/run_MUXParamMaker.C("run", "0084")'
artemis -l 'macro/run_MUXParamMaker.C("run", "0087")'
artemis -l 'macro/run_MUXParamMaker.C("run", "0088")'
artemis -l 'macro/run_MUXParamMaker.C("run", "0090")'
artemis -l 'macro/run_MUXParamMaker.C("run", "0091")'
artemis -l 'macro/run_MUXParamMaker.C("run", "0092")'
artemis -l 'macro/run_MUXParamMaker.C("run", "0093")'
artemis -l 'macro/run_MUXParamMaker.C("run", "0094")'
artemis -l 'macro/run_MUXParamMaker.C("run", "0095")'
artemis -l 'macro/run_MUXParamMaker.C("run", "0096")'
artemis -l 'macro/run_MUXParamMaker.C("run", "0099")'

echo "End converting at $(date)"
