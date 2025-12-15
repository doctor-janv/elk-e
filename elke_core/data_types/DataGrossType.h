#ifndef ELK_E_DATAGROSSTYPE_H
#define ELK_E_DATAGROSSTYPE_H

#include <string>

namespace elke
{

enum class DataGrossType : int
{
  NO_DATA = 0,  ///< An unpopulated data-tree
  SCALAR = 1,   ///< Contains a single value, no children
  SEQUENCE = 2, ///< Contains multiple values, no children
  MAP = 3       ///< Contains no values, multiple children
};

/**Returns a string representation of the type.*/
std::string DataGrossTypeName(DataGrossType type);

} // namespace elke

#endif // ELK_E_DATAGROSSTYPE_H
