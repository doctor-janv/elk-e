#ifndef ELK_E_SCALARINPUTPARAMETER_H
#define ELK_E_SCALARINPUTPARAMETER_H

#include "elke_core/data_types/Varying.h"
#include "elke_core/parameters/InputParameter.h"
#include "elke_core/parameters/ParameterCheck.h"

namespace elke
{

/**Parameter specialization for scalar parameters.*/
class ScalarInputParameter final : public InputParameter
{
  elke::Varying m_default_value;
  elke::Varying m_assigned_value;

public:
  template <typename T>
  ScalarInputParameter(const InputParameterMetaData& meta_data, T default_value)
    : InputParameter(/*name           */ meta_data.m_name,
                     /*parameter_class*/ meta_data.m_class_type,
                     /*gross_type     */ DataGrossType::SCALAR,
                     /*scalar_type    */ Varying(T()).type(),
                     /*description    */ meta_data.m_description,
                     /*checks         */ meta_data.m_checks.empty()
                       ? defaultChecks()
                       : meta_data.m_checks),
      m_default_value(Varying(default_value))
  {
  }

private:
  static std::vector<ParameterCheckPtr> defaultChecks()
  {
    return {std::make_unique<param_checks::ScalarTypeMustBeCompatible>()};
  }

  Varying getScalar() const override { return m_default_value; }
};

} // namespace elke

#endif // ELK_E_SCALARINPUTPARAMETER_H
