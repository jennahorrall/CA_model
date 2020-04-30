#!/bin/bash
#
#SBATCH --job-name=asynchronous_scaling
#SBATCH --output=asynchronous_scal.txt
#SBATCH --nodes=2
#SBATCH --ntasks=16


echo $'\n'
echo "#### STARTING RUN FOR THE ASYNCHRONOUS 2D MODEL ####"
echo "####	     TESTING FOR SCALING              ####"
echo $'\n'

TIMESTEPS=100

cd ../ca_random
make

for i in 254 510 1022 2046 4094 8190
do
   echo "random ind w/ $i x $i matrix:"
   ./rand_ind $i $i $TIMESTEPS
   echo "rand order serial w/ $i x $i matrix:"
   ./rand_ord_serial $i $i $TIMESTEPS

   for j in 1 2 4 8 16
   do
      echo "$j threads, $i x $i matrix"
      ./rand_ord_pthreads $i $i $TIMESTEPS $j
   done

   echo $'\n'
done
