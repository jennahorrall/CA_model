#!/bin/bash
#
#SBATCH --job-name=synchronous_scaling
#SBATCH --output=synchronous_scaling.txt
#SBATCH --nodes=2
#SBATCH --ntasks=16


echo $'\n'
echo "#### STARTING RUN FOR THE SYNCHRONOUS 2D MODEL ####"
echo "####            TESTING FOR SCALING            ####"
echo $'\n'

TIMESTEPS=100


cd ../ca_reg
module load mpi
make


for i in 254 510 1022 2046 4094 8190
do

   echo "serial time for $i x $i matrix, $TIMESTEP timesteps"
   ./ca_serial $i $i $TIMESTEPS
   echo $'\n'

   for j in 1 2 4 8 16
   do
      echo "mpi w/ $j procs: "
      mpirun -np $j ./ca_mpi $i $i $TIMESTEPS

      echo "mpi/openmp w/ $j procs and 4 threads: "
      OMP_NUM_THREADS=4 mpirun -np $j ./ca_mpi_omp $i $i $TIMESTEPS

      echo "pthreads w/ $j threads"
      ./ca_pthreads $i $i $TIMESTEPS $j
      echo $'\n'

   done

   echo $'\n'
done

