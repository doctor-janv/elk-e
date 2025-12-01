#ifndef ELK_E_INPUTPARAMETER_H
#define ELK_E_INPUTPARAMETER_H

#include "elke_core/data_types/DataTree.h"
#include "elke_core/data_types/Varying.h"

#include <string>

namespace elke
{

enum class ParameterClass : int
{
  OPTIONAL = 0,
  REQUIRED = 1,
  DEPRECATED = 2
};

enum class ParameterType : int
{
  SCALAR = 0,
  ARRAY = 1,
  MAP = 2
};

// ###################################################################
/**Base class for input parameters.*/
class InputParameter
{
  const std::string m_name;
  const ParameterClass m_class;
  const ParameterType m_type;
  const std::string m_description;

protected:
  /**Constructor for a base-class input parameter.*/
  InputParameter(std::string name,
                 ParameterClass parameter_class,
                 ParameterType type,
                 std::string description);

public:
  virtual ~InputParameter() = default;
  /**Returns the name of the input parameter.*/
  const std::string& name() const;
  /**Returns the class type of the parameter, i.e. Optional, Required, etc.*/
  const ParameterClass& classType() const;

  /**Returns the value of the parameter.*/
  template <typename T>
  T getValue() const
  {
    auto scalar = getScalar();
    return scalar.getValue<T>();
  }

protected:
  /**Fetches the scalar for the parameters.*/
  virtual Varying getScalar() const { return {}; }
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
  /**Constructor for values not having a default value.*/
  ScalarInputParameter(std::string name,
                       const ParameterClass parameter_class,
                       std::string description)
    : InputParameter(std::move(name),
                     parameter_class,
                     ParameterType::SCALAR,
                     std::move(description)),
      m_default_value(Varying(T())),
      m_scalar_type(m_default_value.type())
  {
  }

  /**Constructor for values WITH a default value.*/
  ScalarInputParameter(std::string name,
                       const ParameterClass parameter_class,
                       T default_value,
                       std::string description)
    : InputParameter(std::move(name),
                     parameter_class,
                     ParameterType::SCALAR,
                     std::move(description)),
      m_default_value(Varying(default_value)),
      m_scalar_type(m_default_value.type())
  {
  }

  /**Returns the scalar type (boolean, integer, real or string).*/
  VaryingDataType scalarType() const { return m_scalar_type; }

protected:
  /**Overriding base class.*/
  Varying getScalar() const override { return m_assigned_value; }
};

} // namespace elke

#endif // ELK_E_INPUTPARAMETER_H
