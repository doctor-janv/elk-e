#include "DataGrossType.h"

namespace elke
{

/**Returns a string representation of the type.*/
std::string DataGrossTypeName(const DataGrossType type)
{
  switch (type)
  {
    case DataGrossType::NO_DATA:
      return "NO_DATA";
    case DataGrossType::SCALAR:
      return "SCALAR";
    case DataGrossType::SEQUENCE:
      return "SEQUENCE";
    case DataGrossType::MAP:
      return "MAP";
    default:
      return "NO_DATA";
  }
}

} // namespace elke