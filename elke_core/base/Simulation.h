#ifndef ELK_E_EXECUTION_CONTEXT_H
#define ELK_E_EXECUTION_CONTEXT_H

#include "elke_core/mpi/MPI_Interface.h"
#include "elke_core/output/OutputInterface.h"
#include "elke_core/cli/CommandLineInterface.h"
#include "elke_core/base/Warehouse.h"

namespace elke
{

/**A base class for modules*/
class Simulation : public MPI_Interface,
                   public OutputInterface,
                   public CommandLineInterface
{
  Warehouse m_warehouse;
  // Constructors/Destructors
public:
  /**Private constructor*/
  explicit Simulation(MPI_Comm communicator);

  virtual ~Simulation() = default;

  // Interface
  const Warehouse& warehouse() const;
  Warehouse& warehouse();
};

} // namespace elke

#endif // ELK_E_EXECUTION_CONTEXT_H
