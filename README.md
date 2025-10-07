# elk-e
General Multiphysics Framework

## Bitterroot installation
First set up the environment.
```
module load openmpi/5.0.5-gcc-13.3.0
module load hpc-code-server/python-3.12
module load cmake/3.30.1

export CC=mpicc
export CXX=mpicxx
export FC=mpifort
mkdir scratch
mkdir scratch/dependencies
```

Then install the dependencies:
```
python tools/configure_dependencies.py -d scratch/dependencies
```

## MacOS installation
Use
`
conda activate moose-dev-2025.08.06
`
