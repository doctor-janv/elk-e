#include "FactoryObject.h"

namespace elke
{

ParameterTree FactoryObject::getInputParameters()
{
  auto params = ParameterTree("FactoryObject",
    "Basic required input parameters for a FactoryObject.");

  params.addRequiredParameter<std::string>("type", "The type of the object.");

  return params;
}

FactoryObject::FactoryObject(const ParameterTree& params)
  // : m_type(params.getParameterValue<std::string>("name"))
{
}

} // namespace elke
