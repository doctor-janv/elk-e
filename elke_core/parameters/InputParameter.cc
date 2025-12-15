#include "InputParameter.h"

#include "elke_core/data_types/DataTree.h"

#include <utility>

namespace elke
{

// ###################################################################
/**Constructor for a base-class input parameter.*/
InputParameter::InputParameter(std::string name,
                               const ParameterClass parameter_class,
                               const DataGrossType type,
                               std::string description,
                               const std::vector<ParameterCheckPtr>& checks)
  : m_name(std::move(name)),
    m_class(parameter_class),
    m_type(type),
    m_description(std::move(description)),
    m_checks(checks)
{
}

/**Returns the name of the input parameter.*/
const std::string& InputParameter::name() const { return m_name; }

/**Returns the class type of the parameter, i.e. Optional, Required, etc.*/
const ParameterClass& InputParameter::classType() const { return m_class; }

/**Returns the type of the parameter.*/
const DataGrossType& InputParameter::grossType() const { return m_type; }

/**Performs all the checks associated with the input parameter. Returns
 * a data structure contains the error and warning strings. */
ParameterCheckResult InputParameter::performChecks(const DataTree& data) const
{
  ParameterCheckResult check_result;

  for (const auto& check_ptr : m_checks)
  {
    const auto result = check_ptr->checkParameter(data, *this);

    check_result.m_error += result.m_error;
    check_result.m_warning += result.m_warning;
  }

  return check_result;
}


} // namespace elke