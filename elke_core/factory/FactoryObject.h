#ifndef ELK_E_FACTORYOBJECT_H
#define ELK_E_FACTORYOBJECT_H

#include "elke_core/parameters/InputParametersBlock.h"

#include <string>

namespace elke
{

class FactoryObject
{
  const std::string m_type;
protected:
  static InputParametersBlock getInputParameters();

  explicit FactoryObject(const InputParametersBlock& params);
};

} // namespace elke

#endif // ELK_E_FACTORYOBJECT_H
