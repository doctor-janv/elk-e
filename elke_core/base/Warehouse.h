#ifndef ELK_E_WAREHOUSE_H
#define ELK_E_WAREHOUSE_H

#include "ItemStorage.h"

namespace elke
{

class DataTree;
class SimulationBlock;

/**The warehouse contains all created object from all avenues.*/
class Warehouse
{
  ItemStorage<DataTree> m_data_tree_storage;
  ItemStorage<SimulationBlock> m_simulation_system_storage;

public:
  Warehouse() = default;

  const ItemStorage<DataTree>& DataTreeStorage() const;
  ItemStorage<DataTree>& DataTreeStorage(); //TODO: make protected

  ItemStorage<SimulationBlock>& SimulationSystemStorage(); //TODO: make protected
};

} // namespace elke

#endif // ELK_E_WAREHOUSE_H
