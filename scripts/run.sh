#!/bin/bash
#
#SBATCH --job-name=ca_model
#SBATCH --output=output.txt
#SBATCH --nodes=1
#SBATCH --ntasks=1


ROWS=10000
COLS=10000

echo $'\n'
echo "#### STARTING RUN for size $ROWS x $COLS ####"
echo $'\n'


cd ..

./ca_model $ROWS $COLS
