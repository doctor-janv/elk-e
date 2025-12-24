#ifndef ELK_E_INPUTPARAMETER_H
#define ELK_E_INPUTPARAMETER_H

#include "elke_core/data_types/DataTree.h"
#include "elke_core/data_types/Varying.h"
#include "elke_core/data_types/DataGrossType.h"
#include "ParameterCheck.h"

#include <string>

namespace elke
{

class DataTree;

enum class ParameterClass : int
{
  OPTIONAL = 0,
  REQUIRED = 1,
  DEPRECATED = 2
};

using ParameterCheckPtr = std::shared_ptr<param_checks::ParameterCheck>;

// ###################################################################
/**Base class for input parameters.*/
class InputParameter
{
  const std::string m_name;
  const ParameterClass m_class;
  const DataGrossType m_type;
  const std::string m_description;

  const std::vector<ParameterCheckPtr> m_checks;

protected:
  /**Constructor for a base-class input parameter.*/
  InputParameter(std::string name,
                 ParameterClass parameter_class,
                 DataGrossType type,
                 std::string description,
                 const std::vector<ParameterCheckPtr>& checks);

public:
  virtual ~InputParameter() = default;
  /**Returns the name of the input parameter.*/
  const std::string& name() const;
  /**Returns the class type of the parameter, i.e. Optional, Required, etc.*/
  const ParameterClass& classType() const;

  /**Returns the type of the parameter.*/
  const DataGrossType& grossType() const;

  /**Returns the value of the parameter.*/
  template <typename T>
  T getValue() const
  {
    auto scalar = getScalar();
    return scalar.getValue<T>();
  }

  /**Performs all the checks associated with the input parameter.*/
  ParameterCheckResult performChecks(const DataTree& data) const;

  /**Fetches the scalar for the parameters.*/
  virtual Varying getScalar() const { return {}; }
  virtual VaryingDataType scalarType() const { return VaryingDataType::VOID; }
};

// ###################################################################
/**A scalar input parameter for handling scalars only.*/
template <typename T>
class ScalarInputParameter final : public InputParameter
{
  Varying m_default_value;             ///< Default value to assign
  const VaryingDataType m_scalar_type; ///< Scalar type
  Varying m_assigned_value;            ///< Assigned value (after assignment)
  bool m_has_been_assigned = false;    ///< Flag to check for assignment

public:
  /**Constructor for scalar values.*/
  explicit ScalarInputParameter(std::string name,
                                const ParameterClass parameter_class,
                                T default_value,
                                std::string description,
                                const std::vector<ParameterCheckPtr>& checks)
    : InputParameter(std::move(name),
                     parameter_class,
                     DataGrossType::SCALAR,
                     std::move(description),
                     checks),
      m_default_value(Varying(default_value)),
      m_scalar_type(m_default_value.type())
  {
  }

  /**Returns the scalar type (boolean, integer, real or string).*/
  VaryingDataType scalarType() const override { return m_scalar_type; }

protected:
  /**Overriding base class.*/
  Varying getScalar() const override { return m_assigned_value; }
};

// ###################################################################
/**An abstract array input parameter.*/
template <typename T>
class SequenceInputParameter final : public InputParameter
{
  std::vector<Varying> m_default_value;  ///< Default value to assign
  std::vector<Varying> m_assigned_value; ///< Assigned value (after assignment)
  bool m_has_been_assigned = false;      ///< Flag to check for assignment

public:
  /**Constructor for array values.*/
  explicit SequenceInputParameter(
    std::string name,
    const ParameterClass parameter_class,
    const std::enable_if_t<IsVector<T>::value, T>& default_value,
    std::string description,
    const std::vector<ParameterCheckPtr>& checks)
    : InputParameter(std::move(name),
                     parameter_class,
                     DataGrossType::SEQUENCE,
                     std::move(description),
                     checks),
      m_default_value(convertVectorTtoVectorVarying(default_value))
  {
  }

private:
  static std::vector<Varying>
  convertVectorTtoVectorVarying(const T& default_value)
  {
    std::vector<Varying> var_vec;
    for (const auto& value : default_value)
      var_vec.emplace_back(value);

    return var_vec;
  }
};

} // namespace elke

#endif // ELK_E_INPUTPARAMETER_H
