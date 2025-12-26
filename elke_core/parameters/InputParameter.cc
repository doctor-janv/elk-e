#include "InputParameter.h"

#include "elke_core/data_types/DataTree.h"

namespace elke
{

/**Returns the name of the input parameter.*/
const std::string& InputParameter::name() const { return m_name; }

/**Returns the class type of the parameter, i.e. Optional, Required, etc.*/
const ParameterClass& InputParameter::classType() const { return m_class_type; }

/**Returns the type of the parameter.*/
const DataGrossType& InputParameter::grossType() const { return m_gross_type; }

/**Performs all the checks associated with the input parameter. Returns
 * a data structure contains the error and warning strings. */
ParameterCheckResult
InputParameter::performChecks(const DataTree& data,
                              const unsigned int nest_depth) const
{
  ParameterCheckResult check_result;


  for (const auto& check_ptr : m_checks)
  {
    const auto result = check_ptr->checkParameter(data, *this, nest_depth);

    check_result.m_error += result.m_error;
    check_result.m_warning += result.m_warning;
  }

  return check_result;
}

/**Fetches the scalar for the parameters.*/
Varying InputParameter::getScalar() const
{
  if (m_gross_type == DataGrossType::SCALAR) return m_assigned_data.scalar_data;

  throw std::logic_error(
    "Attempting to retrieve scalar for input parameter \"" + m_name +
    "\" failed because the parameter is not of gross-type SCALAR. It has "
    "gross-type " +
    DataGrossTypeName(m_gross_type) + ".");
}

/**Returns the scalar type if the parameter is a scalar, otherwise
 * VaryingDataType::VOID.*/
VaryingDataType InputParameter::scalarType() const
{
  switch (m_gross_type)
  {
    case DataGrossType::SCALAR:
      return m_default_data.scalar_data.type();
    case DataGrossType::NO_DATA:
    case DataGrossType::SEQUENCE:
    case DataGrossType::MAP:
    default:
      return VaryingDataType::VOID;
  }
}

} // namespace elke