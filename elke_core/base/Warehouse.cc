#include "Warehouse.h"

namespace elke
{

ItemStorage<DataTree>& Warehouse::DataTreeStorage()
{
  return m_data_tree_storage;
}

const ItemStorage<DataTree>& Warehouse::DataTreeStorage() const
{
  return m_data_tree_storage;
}

ItemStorage<SimulationBlock>& Warehouse::SimulationSystemStorage()
{
  return m_simulation_system_storage;
}

} // namespace elke
