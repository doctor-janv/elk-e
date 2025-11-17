# Elk-e Framework

Welcome to the developer's manual for the ELKE framework.

## Quick links:
- \ref DesignRequirements
- \ref CodingConventions
- \ref RTM

## 1. Execution principles
Execution of an elke based application starts with initializing and executing 
the `elke::FrameworkCore`.

Here is an example using a `c++` program:
```c++
#include "elke_core/FrameworkCore.h"

int main(const int argc, char** argv)
{
  elke::FrameworkCore::initialize(argc, argv);
  return elke::FrameworkCore::execute();
}
```

## 2. High-level structure

The top-level object is the singleton `elke::FrameworkCore`. The core has many
functions but a high-level it:
- Receives, and initially responds to, command line arguments (CLAs).
- Initializes the Message Passing Interface (MPI) system for multiprocessing.
