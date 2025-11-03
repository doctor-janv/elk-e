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

} // namespace elke
