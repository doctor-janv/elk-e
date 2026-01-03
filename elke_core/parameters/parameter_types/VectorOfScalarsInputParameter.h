#ifndef ELK_E_VECTOROFSCALARSINPUTPARAMETER_H
#define ELK_E_VECTOROFSCALARSINPUTPARAMETER_H

#include "elke_core/data_types/Varying.h"
#include "elke_core/parameters/InputParameter.h"
#include "elke_core/parameters/ParameterCheck.h"

namespace elke
{

/**Parameter specialization for a vector of scalars. This class
 * has some "fluff" built-in because of the platform specific deduction
 * of `bool` vectors.
 */
class VectorOfScalarsInputParameter final : public InputParameter
{
  std::vector<elke::Varying> m_default_values;
  std::vector<elke::Varying> m_assigned_values;

public:
  template <typename T>
  VectorOfScalarsInputParameter(const InputParameterMetaData& meta_data,
                                T default_value)
    : InputParameter(/*name           */ meta_data.m_name,
                     /*parameter_class*/ meta_data.m_class_type,
                     /*gross_type     */ DataGrossType::SEQUENCE,
                     /*scalar_type    */ makeScalarType(default_value),
                     /*description    */ meta_data.m_description,
                     /*checks         */ meta_data.m_checks.empty()
                       ? defaultChecks()
                       : meta_data.m_checks),
      m_default_values(makeVaryingVector(default_value))
  {
  }

private:
  static std::vector<ParameterCheckPtr> defaultChecks()
  {
    return {
      std::make_unique<param_checks::ScalarArrayEntriesTypeMustBeCompatible>()};
  }
  template <typename V>
  static std::vector<Varying> makeVaryingVector(const std::vector<V>& vec)
  {
    std::vector<Varying> output;
    for (const auto& val : vec)
      output.push_back(Varying(val));

    return output;
  }

  static std::vector<Varying> makeVaryingVector(const std::vector<bool>& vec)
  {
    std::vector<Varying> output;
    for (const auto val : vec)
      output.push_back(Varying(static_cast<bool>(val)));

    return output;
  }

  template <typename V>
  static VaryingDataType makeScalarType(const std::vector<V>&)
  {
    return Varying(V()).type();
  }

  static VaryingDataType makeScalarType(const std::vector<bool>&)
  {
    return VaryingDataType::BOOL;
  }
};

} // namespace elke

#endif // ELK_E_VECTOROFSCALARSINPUTPARAMETER_H
