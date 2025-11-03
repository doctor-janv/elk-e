#include "Simulation.h"

namespace elke
{

Simulation::Simulation(const MPI_Comm communicator)
  : MPI_Interface(communicator),
    OutputInterface(this->rank()),
    CommandLineInterface(this->getLoggerPtr())
{
}

// ###################################################################
const Warehouse& Simulation::warehouse() const { return m_warehouse; }

// ###################################################################
Warehouse& Simulation::warehouse() { return m_warehouse; }
} // namespace elke