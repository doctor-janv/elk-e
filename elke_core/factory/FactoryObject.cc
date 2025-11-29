#include "FactoryObject.h"

namespace elke
{

InputParametersBlock FactoryObject::getInputParameters()
{
  auto params = InputParametersBlock();

  params.addRequiredParameter<std::string>("type", "The type of the object.");

  return params;
}

FactoryObject::FactoryObject(const InputParametersBlock& params)
  : m_type(params.getParameterValue<std::string>("name"))
{
}

} // namespace elke
