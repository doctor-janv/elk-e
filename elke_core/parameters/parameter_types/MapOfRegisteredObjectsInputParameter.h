#ifndef ELK_E_MAPOFREGISTEREDOBJECTSINPUTPARAMETER_H
#define ELK_E_MAPOFREGISTEREDOBJECTSINPUTPARAMETER_H

#include "elke_core/data_types/Varying.h"
#include "elke_core/parameters/InputParameter.h"
#include "elke_core/parameters/ParameterCheck.h"

namespace elke
{

/**Parameter specialization for scalar parameters.*/
class MapOfRegisteredObjectsInputParameter final : public InputParameter
{
public:
  template <typename T>
  MapOfRegisteredObjectsInputParameter(
    const InputParameterMetaData& meta_data, T _)
    : InputParameter(/*name           */ meta_data.m_name,
                     /*parameter_class*/ meta_data.m_class_type,
                     /*gross_type     */ DataGrossType::MAP,
                     /*scalar_type    */ VaryingDataType::VOID,
                     /*description    */ meta_data.m_description,
                     /*checks         */ meta_data.m_checks.empty()
                       ? defaultChecks()
                       : meta_data.m_checks)
  {
  }

private:
  static std::vector<ParameterCheckPtr> defaultChecks()
  {
    return {
      std::make_unique<param_checks::RegisteredObjectMapEntriesMustExist>()};
  }
};

} // namespace elke

#endif // ELK_E_MAPOFREGISTEREDOBJECTSINPUTPARAMETER_H
