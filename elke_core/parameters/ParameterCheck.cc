#include "ParameterCheck.h"
#include "elke_core/data_types/DataGrossType.h"
#include "InputParameter.h"

namespace elke::param_checks
{

ParameterCheckResult
GrossTypeMustMatch::checkParameter(const DataTree& data,
                                   const InputParameter& parameter)
{
  ParameterCheckResult result;

  if (data.grossType() != parameter.grossType())
  {
    result.m_error +=
      std::string("GrossTypeMustMatch\n") + "Item \"" + data.name() +
      "\" is required to be of type " + DataGrossTypeName(parameter.grossType()) +
      ". Supplied type is " + DataGrossTypeName(data.grossType()) + ".\n";
  }

  return result;
}

ParameterCheckResult
ScalarTypeMustMatch::checkParameter(const DataTree& data,
                                    const InputParameter& parameter)
{
  auto check_result = m_gross_check.checkParameter(data, parameter);

  if (not check_result.m_error.empty())
  {
    check_result.m_error = "ScalarTypeMustMatch\n" + check_result.m_error;
    return check_result;
  }

  const auto& data_value = data.value();
  const auto param_type = parameter.scalarType();

  if (data_value.type() != param_type)
  {
    check_result.m_error +=
      std::string("ScalarTypeMustMatch\n") + "Item \"" + data.name() +
      "\" is required to be of type " + varyingDataTypeStringName(param_type) +
      ". Supplied type is " + data_value.typeName() + ".\n";
  }

  return check_result;
}

} // namespace elke::param_checks