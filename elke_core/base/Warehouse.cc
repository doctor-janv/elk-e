#include "Warehouse.h"

#include <iostream>

namespace elke
{

Warehouse& Warehouse::getInstance() noexcept
{
  static Warehouse instance;
  return instance;
}

}
