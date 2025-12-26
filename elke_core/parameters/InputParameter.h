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
using DataTreePtr = std::shared_ptr<DataTree>;

enum class ParameterClass : int
{
  OPTIONAL = 0,
  REQUIRED = 1,
  DEPRECATED = 2
};

using ParameterCheckPtr = std::shared_ptr<param_checks::ParameterCheck>;

struct InputParameterData
{
  Varying scalar_data;
  std::vector<InputParameterData> sequence_data;
  DataTreePtr factory_object_data;
  std::vector<DataTreePtr> factor_object_array_data;
};

class RegisteredObjectProxy
{
};

template <typename T>
struct IsRegisteredObjectProxy
{
  static constexpr bool value = std::is_same_v<T, RegisteredObjectProxy>;
};

template <typename T>
struct IsRegisteredObjectProxyArray
{
  static constexpr bool value =
    std::is_same_v<T, std::vector<RegisteredObjectProxy>>;
};

// ###################################################################
/**Base class for input parameters.*/
class InputParameter final
{
  const std::string m_name;
  const ParameterClass m_class_type;
  const DataGrossType m_gross_type;
  const std::string m_description;

  const InputParameterData m_default_data;

  const std::vector<ParameterCheckPtr> m_checks;

  InputParameterData m_assigned_data;

public:
  /**Constructor for scalar values*/
  template <typename T, typename = std::enable_if_t<IsScalar<T>::value, T>>
  InputParameter(std::string name,
                 const ParameterClass parameter_class,
                 T default_value,
                 std::string description,
                 const std::vector<ParameterCheckPtr>& checks)
    : m_name(std::move(name)),
      m_class_type(parameter_class),
      m_gross_type(DataGrossType::SCALAR),
      m_description(std::move(description)),
      m_default_data{Varying(default_value), {}, {}, {}},
      m_checks(checks)
  {
  }

  /**Constructor for scalar-array values.*/
  template <typename T>
  InputParameter(std::string name,
                 const ParameterClass parameter_class,
                 std::vector<T> _,
                 std::string description,
                 const std::vector<ParameterCheckPtr>& checks)
    : m_name(std::move(name)),
      m_class_type(parameter_class),
      m_gross_type(DataGrossType::SCALAR),
      m_description(std::move(description)),
      m_default_data{Varying(T()), {}, {}, {}},
      m_checks(checks)
  {
  }

  /**Constructor for Registered Object entries.*/
  InputParameter(std::string name,
                 const ParameterClass parameter_class,
                 RegisteredObjectProxy _,
                 std::string description,
                 const std::vector<ParameterCheckPtr>& checks)
    : m_name(std::move(name)),
      m_class_type(parameter_class),
      m_gross_type(DataGrossType::MAP),
      m_description(std::move(description)),
      m_default_data{Varying(), {}, {}, {}},
      m_checks(checks)
  {
  }

  /**Constructor for an array of Registered Object entries.*/
  InputParameter(std::string name,
                 const ParameterClass parameter_class,
                 const std::vector<RegisteredObjectProxy>& _,
                 std::string description,
                 const std::vector<ParameterCheckPtr>& checks)
    : m_name(std::move(name)),
      m_class_type(parameter_class),
      m_gross_type(DataGrossType::MAP),
      m_description(std::move(description)),
      m_default_data{Varying(), {}, {}, {}},
      m_checks(checks)
  {
  }

  ~InputParameter() = default;
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
  ParameterCheckResult performChecks(const DataTree& data,
                                     unsigned int nest_depth) const;

  /**Returns the scalar type if the parameter is a scalar, otherwise
   * VaryingDataType::VOID.*/
  VaryingDataType scalarType() const;

private:
  /**Fetches the scalar for the parameters.*/
  Varying getScalar() const;
};

} // namespace elke

#endif // ELK_E_INPUTPARAMETER_H
