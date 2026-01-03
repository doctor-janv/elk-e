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

ItemStorage<SyntaxBlock>& Warehouse::SyntaxBlockStorage()
{
  return m_syntax_block_storage;
}

} // namespace elke
