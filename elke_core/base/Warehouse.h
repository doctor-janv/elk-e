#ifndef ELK_E_WAREHOUSE_H
#define ELK_E_WAREHOUSE_H

#include "ItemStorage.h"

namespace elke
{

class DataTree;

/**The warehouse contains all created object from all avenues.*/
class Warehouse
{
  ItemStorage<DataTree> m_data_tree_storage;

public:
  Warehouse() = default;

  const ItemStorage<DataTree>& DataTreeStorage() const;
   ItemStorage<DataTree>& DataTreeStorage();
};

} // namespace elke

#endif // ELK_E_WAREHOUSE_H
