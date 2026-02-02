#ifndef ELK_E_FACTORYOBJECT_H
#define ELK_E_FACTORYOBJECT_H

#include "elke_core/parameters2/ParameterTree.h"

#include <string>

namespace elke
{

class FactoryObject
{
  const std::string m_type = "none";
protected:
  static ParameterTree getInputParameters();

  explicit FactoryObject(const ParameterTree& params);
};

} // namespace elke

#endif // ELK_E_FACTORYOBJECT_H
