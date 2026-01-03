#ifndef ELK_E_VECTOROFNAMEDBLOCKSINPUTPARAMETER_H
#define ELK_E_VECTOROFNAMEDBLOCKSINPUTPARAMETER_H

#include "elke_core/data_types/Varying.h"
#include "elke_core/parameters/InputParameter.h"
#include "elke_core/parameters/ParameterCheck.h"

namespace elke
{

/**Parameter specialization for a named input block.*/
class VectorOfNamedBlocksInputParameter final : public InputParameter
{
  const std::string m_block_name;

public:
  VectorOfNamedBlocksInputParameter(const InputParameterMetaData& meta_data,
                                    std::string name_of_blocks)
    : InputParameter(/*name           */ meta_data.m_name,
                     /*parameter_class*/ meta_data.m_class_type,
                     /*gross_type     */ DataGrossType::SEQUENCE,
                     /*scalar_type    */ VaryingDataType::VOID,
                     /*description    */ meta_data.m_description,
                     /*checks         */ meta_data.m_checks.empty()
                       ? defaultChecks()
                       : meta_data.m_checks),
      m_block_name(std::move(name_of_blocks))
  {
  }

  std::string blockName() const { return m_block_name; }

private:
  static std::vector<ParameterCheckPtr> defaultChecks()
  {
    return {std::make_unique<param_checks::ArrayOfNamedInputBlocksCheck>()};
  }
};

} // namespace elke

#endif // ELK_E_VECTOROFNAMEDBLOCKSINPUTPARAMETER_H
