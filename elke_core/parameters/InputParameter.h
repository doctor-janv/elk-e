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

// ######################################################### Helpers
/**A dummy class to indicate a template needs to specialize for the input
 * parameters of an object.*/
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

template <typename T>
struct IsRegisteredObjectProxyMap
{
  static constexpr bool value =
    std::is_same_v<T, std::map<std::string, RegisteredObjectProxy>>;
};

enum class ParameterClass : int
{
  OPTIONAL = 0,
  REQUIRED = 1,
  DEPRECATED = 2
};

using ParameterCheckPtr = std::shared_ptr<param_checks::ParameterCheck>;

/**A simple data structure to possible data.*/
struct InputParameterData
{
  Varying scalar_data;
  std::vector<InputParameterData> sequence_data;
  DataTreePtr data_for_single_input_block;
  std::vector<DataTreePtr> data_for_array_of_input_blocks;
};

/**A simple data structure for basic meta-data.*/
struct InputParameterMetaData
{
  const std::string m_name;
  const ParameterClass m_class_type;
  const std::string m_description;
  const std::vector<ParameterCheckPtr> m_checks;
};

// ###################################################################
/**A basic input parameter. .*/
class InputParameter
{
  const std::string m_name;
  const ParameterClass m_class_type;
  const DataGrossType m_gross_type;
  const VaryingDataType m_scalar_type;
  const std::string m_description;

  const InputParameterData m_default_data;

  const std::vector<ParameterCheckPtr> m_checks;

  const std::string m_block_name;

  InputParameterData m_assigned_data;

  /**Private copy constructor.*/
  InputParameter(const InputParameter& rhs)
    : m_name(rhs.m_name),
      m_class_type(rhs.m_class_type),
      m_gross_type(rhs.m_gross_type),
      m_scalar_type(rhs.m_scalar_type),
      m_description(rhs.m_description),
      m_default_data(rhs.m_default_data),
      m_checks(rhs.m_checks),
      m_block_name(rhs.m_block_name),
      m_assigned_data(rhs.m_assigned_data)
  {
  }

protected:
  InputParameter(std::string name,
                 const ParameterClass parameter_class,
                 const DataGrossType gross_type,
                 const VaryingDataType scalar_type,
                 std::string description,
                 const std::vector<ParameterCheckPtr>& checks)
    : m_name(std::move(name)),
      m_class_type(parameter_class),
      m_gross_type(gross_type),
      m_scalar_type(scalar_type),
      m_description(std::move(description)),
      m_checks(checks)
  {
  }

public:
  virtual ~InputParameter() = default;

  std::unique_ptr<InputParameter> clone() const
  {
    return std::unique_ptr<InputParameter>(new InputParameter(*this));
  }

  /**Constructor for a named input parameter block*/
  InputParameter(std::string name,
                 const ParameterClass parameter_class,
                 std::string description,
                 const std::vector<ParameterCheckPtr>& checks,
                 std::string block_name)
    : m_name(std::move(name)),
      m_class_type(parameter_class),
      m_gross_type(DataGrossType::MAP),
      m_scalar_type(VaryingDataType::VOID),
      m_description(std::move(description)),
      m_default_data{Varying(), {}, {}, {}},
      m_checks(checks),
      m_block_name(std::move(block_name))
  {
  }

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

  /**Returns the name of the block to match (if specified).*/
  const std::string& getBlockNameMetaData() const { return m_block_name; }

private:
  /**Fetches the scalar for the parameters.*/
  virtual Varying getScalar() const;
};

} // namespace elke

#endif // ELK_E_INPUTPARAMETER_H
