#include "Factory.h"

#include "elke_core/syntax_blocks/SimulationBlock.h"
#include "elke_core/parameters/InputParametersBlock.h"

namespace elke
{

Factory::Factory(Warehouse& warehouse) : m_warehouse(warehouse) {}

std::shared_ptr<SimulationBlock>
Factory::makeSimulationSystem(const std::string& registered_name,
                              const DataTree& unchecked_parameters) const
{
  auto input_parameters = InputParametersBlock();
  auto system = std::make_shared<SimulationBlock>(input_parameters);

  m_warehouse.SimulationSystemStorage().depositItem(system);

  return system;
}

} // namespace elke