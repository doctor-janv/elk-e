#ifndef ELK_E_FACTORY_H
#define ELK_E_FACTORY_H

#include "elke_core/base/Warehouse.h"

#include <memory>


namespace elke
{

class SimulationBlock;
class DataTree;

/**A generic factor for constructing objects based on input parameters.*/
class Factory
{
  Warehouse& m_warehouse;

public:
  explicit Factory(Warehouse& warehouse);

  std::shared_ptr<SimulationBlock>
  makeSimulationSystem(const std::string& registered_name,
                       const DataTree& unchecked_parameters) const;
};

} // namespace elke

#endif // ELK_E_FACTORY_H
