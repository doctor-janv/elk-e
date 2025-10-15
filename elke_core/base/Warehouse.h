#ifndef ELK_E_WAREHOUSE_H
#define ELK_E_WAREHOUSE_H

#include "ItemStack.h"

namespace elke
{

class DataTree;

/**The warehouse contains all created object from all avenues.*/
class Warehouse
{
public:
  ItemStack<DataTree> m_data_tree_stack;

  static Warehouse& getInstance() noexcept;

private:
  Warehouse() = default;
};

} // namespace elke

#endif // ELK_E_WAREHOUSE_H
