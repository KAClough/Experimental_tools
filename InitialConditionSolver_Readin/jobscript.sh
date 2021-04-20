#!/bin/bash -l
#SBATCH -A dp174
#SBATCH -p cosma6-pauper
#SBATCH --nodes 1
### NB cosma6 has 16 cores per node so product of these = 16
#SBATCH --ntasks-per-node=4
#SBATCH --cpus-per-task=4
#SBATCH -o output_file.%J.out
#SBATCH -e error_file.%J.err
#SBATCH -t 2:00:00
#SBATCH -J KC_ICs
#SBATCH --exclusive
#SBATCH --mail-type=ALL                          # notifications for job done & fail
#SBATCH --mail-user=katy.clough@physics.ox.ac.uk

module purge
#load the modules used to build your program.
module load intel_comp/2019 intel_mpi/2019 parallel_hdf5/1.10.3

export OMP_NUM_THREADS=$SLURM_CPUS_PER_TASK

# Run the program
mpirun -np $SLURM_NTASKS ./Main_PoissonSolver3d.Linux.64.mpiicpc.ifort.OPT.MPI.OPENMPCC.ex params.txt
