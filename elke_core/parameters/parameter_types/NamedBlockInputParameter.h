#ifndef ELK_E_NAMEDBLOCKINPUTPARAMETER_H
#define ELK_E_NAMEDBLOCKINPUTPARAMETER_H

#include "elke_core/data_types/Varying.h"
#include "elke_core/parameters/InputParameter.h"
#include "elke_core/parameters/ParameterCheck.h"

namespace elke
{

/**Parameter specialization for a named input block.*/
class NamedBlockInputParameter final : public InputParameter
{
  const std::string m_block_name;

public:
  NamedBlockInputParameter(const InputParameterMetaData& meta_data,
                           std::string block_name)
    : InputParameter(/*name           */ meta_data.m_name,
                     /*parameter_class*/ meta_data.m_class_type,
                     /*gross_type     */ DataGrossType::MAP,
                     /*scalar_type    */ VaryingDataType::VOID,
                     /*description    */ meta_data.m_description,
                     /*checks         */ meta_data.m_checks.empty()
                       ? defaultChecks()
                       : meta_data.m_checks),
      m_block_name(std::move(block_name))
  {
  }

  std::string blockName() const { return m_block_name; }

private:
  static std::vector<ParameterCheckPtr> defaultChecks()
  {
    return {std::make_unique<param_checks::NamedInputBlockCheck>()};
  }
};

} // namespace elke

#endif // ELK_E_NAMEDBLOCKINPUTPARAMETER_H
