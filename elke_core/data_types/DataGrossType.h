#ifndef ELK_E_DATAGROSSTYPE_H
#define ELK_E_DATAGROSSTYPE_H

#include <string>
#include <sstream>

namespace elke
{

enum class DataGrossType : int
{
  NO_DATA = 0,  ///< An unpopulated data-tree
  SCALAR = 1,   ///< Contains a single value, no children
  SEQUENCE = 2, ///< Contains multiple unnamed children
  MAP = 3       ///< Contains multiple named children
};

/**Returns a string representation of the type.*/
std::string DataGrossTypeName(DataGrossType type);

/**Returns a string representation of a collection of types. The collection
 * can be any container with an iterator.
 */
template<typename C>
std::string DataGrossTypeNames(const C& collection)
{
  std::stringstream output;
  output << "[";
  for (const auto& item : collection)
  {
    output << DataGrossTypeName(item);
    if (item != collection.back()) output << ",";
  }
  output << "]";

  return output.str();
}
} // namespace elke

#endif // ELK_E_DATAGROSSTYPE_H
