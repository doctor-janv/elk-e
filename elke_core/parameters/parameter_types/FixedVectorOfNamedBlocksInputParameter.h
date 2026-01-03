#ifndef ELK_E_FIXEDVECTOROFNAMEDBLOCKSINPUTPARAMETER_H
#define ELK_E_FIXEDVECTOROFNAMEDBLOCKSINPUTPARAMETER_H

#include "elke_core/data_types/Varying.h"
#include "elke_core/parameters/InputParameter.h"
#include "elke_core/parameters/ParameterCheck.h"

namespace elke
{

/**Parameter specialization for a named input block.*/
class FixedVectorOfNamedBlocksInputParameter final : public InputParameter
{
  const std::vector<std::string> m_block_names;

public:
  FixedVectorOfNamedBlocksInputParameter(
    const InputParameterMetaData& meta_data,
    std::vector<std::string> block_names)
    : InputParameter(/*name           */ meta_data.m_name,
                     /*parameter_class*/ meta_data.m_class_type,
                     /*gross_type     */ DataGrossType::SEQUENCE,
                     /*scalar_type    */ VaryingDataType::VOID,
                     /*description    */ meta_data.m_description,
                     /*checks         */ defaultChecks()),
      m_block_names(std::move(block_names))
  {
  }

  const std::vector<std::string>& blockNames() const { return m_block_names; }

private:
  static std::vector<ParameterCheckPtr> defaultChecks()
  {
    return {std::make_unique<param_checks::FixedArrayOfNamedInputBlocksCheck>()};
    // return {};
  }
};

} // namespace elke

#endif // ELK_E_FIXEDVECTOROFNAMEDBLOCKSINPUTPARAMETER_H
