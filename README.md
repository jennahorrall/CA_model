# Cellular Automata Synchronous/Asynchronous Model

A 2D cellular automata model. We have implementations of both a synchronous and stochastic, asynchronous model. 
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


## ca_random
This directory contains code for the randomized, asynchronous version of the 2D model. This means that instead of having each cell's state update together synchronously, in this version each new cell update affects the computation of neighboring cells. On top of that, the cells have to be updated stochastically, or at random. Here are the different executables and their descriptions.


```
./ca_model 50 50
```



Where the first argument is the number of rows and the second argument is the number of columns.

Run the run.sh file from the "scripts" folder on the cluster using sbatch:

```
sbatch run.sh
```


## Authors

* **Paul Bailey** 
* **Callan Hand** 
* **Jenna Horrall** 



## Acknowledgments

* The Nature of Code, Daniel Shiffman.
* Kelly Kochanski.

