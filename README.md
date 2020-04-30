# Cellular Automata Synchronous/Asynchronous Model

2D cellular automata model. We have implementations of both a synchronous and asynchronous 2D model. 
<br />
<br /> Source code link: 
<br /> https://github.com/jennahorrall/CA_model.git

## Getting Started

Clone this github onto your local machine. 

### Prerequisites

If you would like to run the testing scripts, you will need access to a computing cluster. You will need to have MPI and OpenMP installed on those machines.

### Installing

Once you've cloned the github repo, cd into either the "ca_reg" or "ca_random" folder. The ca_reg folder contains both serial and parallel implementations of the synchronous 2D model. The ca_random folder contains an attempted parallel implementation and a serial version of the asynchronous, stochastic 2D model.

```
make
```

If you get an error that mentions "mpicc module not found," type this and run make again:

```
module load mpi
```


After the make is successful, you will see several different executables. See below for descriptions of each.


## ca_reg
This directory contains the code for the synchronous 2D cellular automata model. During each timestep, all cells are updated together before the new states affect other cells. 


* ca_serial: serial implementation of a synchronous cellular automata model.
* ca_pthreads: pthread implementation of the model - worker threads each compute a section of the updates in the cellspace.
* ca_mpi: MPI implementation. Each process computes a local section of the updates in the cellspace and gathered into the global cellspace after each timestep.
* ca_mpi_omp: This is a hybrid MPI/OpenMP implementation of the model. Each process does it's local updates, and those updates occur in parallel using OpenMP.

To run these different executables:

```
mpirun -np <nprocs> ./ca_mpi <rows> <cols> <timesteps>
mpirun -np <nprocs> ./ca_mpi_omp <rows> <cols> <timesteps>
./ca_serial <rows> <cols> <timesteps>
./ca_pthreads <rows> <cols> <timesteps> <nthreads>
```

## ca_random
This directory contains code for the randomized, asynchronous version of the 2D model. This means that instead of having each cell's state update together synchronously, in this version each new cell update affects the computation of neighboring cells. The cells are updated stochastically, or at random. Below are the different executables and their descriptions.

* rand_ind: this model uses the random independent scheme - one cell per timestep is updated.
* rand_ord_serial: this model uses the random ordering scheme - all cells are updated in random order.
* rand_ord_pthreads: this model takes advantage of pthreads at an attempt to parallelize the serial version (rand_ord_serial).

To run these executables:

```
./rand_ind <rows> <cols> <timesteps>
./rand_ord_serial <rows> <cols> <timesteps>
./rand_ord_pthreads <rows> <cols> <timesteps> <num_threads>
```

Keep in mind that the "rand_ord_pthreads" executable expects an extra argument for the amount of threads.

If you would like to run our scripts for large matrices, we have made sbatch scripts available. To run these scripts, make sure you are on a cluster machine and run the following command using an sbatch script:

```
sbatch <script.sh>
```


## Authors

* **Paul Bailey** 
* **Callan Hand** 
* **Jenna Horrall** 



## Acknowledgments

* The Nature of Code, Daniel Shiffman.
* Kelly Kochanski.

