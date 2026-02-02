#include "Factory.h"

#include "elke_core/syntax_blocks/SimulationBlock.h"
#include "elke_core/parameters2/ParameterTree.h"

namespace elke
{

Factory::Factory(Warehouse& warehouse) : m_warehouse(warehouse) {}

std::shared_ptr<SimulationBlock>
Factory::makeSimulationSystem(const std::string& registered_name,
                              const DataTree& unchecked_parameters) const
{
  auto input_parameters = ParameterTree("dummy", "dummy");
  auto system = std::make_shared<SimulationBlock>(input_parameters);

  m_warehouse.SyntaxBlockStorage().depositItem(system);

  return system;
}

} // namespace elke