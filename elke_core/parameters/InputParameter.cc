#include "InputParameter.h"

#include <utility>

namespace elke
{

// ###################################################################
/**Constructor for a base-class input parameter.*/
InputParameter::InputParameter(std::string name,
                               const ParameterClass parameter_class,
                               const ParameterType type,
                               std::string description)
  : m_name(std::move(name)),
    m_class(parameter_class),
    m_type(type),
    m_description(std::move(description))
{
}

/**Returns the name of the input parameter.*/
const std::string& InputParameter::name() const { return m_name; }

/**Returns the class type of the parameter, i.e. Optional, Required, etc.*/
const ParameterClass& InputParameter::classType() const { return m_class; }

} // namespace elke