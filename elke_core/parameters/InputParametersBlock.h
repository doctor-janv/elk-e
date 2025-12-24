#ifndef ELK_E_INPUTPARAMETERS_H
#define ELK_E_INPUTPARAMETERS_H

#include "InputParameter.h"
#include "ParameterCheck.h"
#include <vector>

namespace elke
{

using InputParameterPtr = std::shared_ptr<InputParameter>;

class InputParametersBlock
{
  const std::string m_name;
  const std::string m_block_description;
  std::vector<InputParameterPtr> m_parameters;

public:
  /**Creates a barebones empty block.*/
  InputParametersBlock(
    std::string name,
    std::string description,
    const std::vector<InputParametersBlock>& parent_blocks = {});

  const std::string& name() const { return m_name; }

  /**Adds an optional parameter.*/
  template <typename T>
  void addOptionalParameter(std::string name,
                            T default_value,
                            std::string description,
                            std::vector<ParameterCheckPtr> checks = {})
  {
    if (this->hasParameter(name))
      throw std::runtime_error(m_name + ": Trying to add parameter \"" + name +
                               "\", but the parameter already exists.");

    auto pointer = makeTemplatedInputParameter(
      name, ParameterClass::OPTIONAL, default_value, description, checks);

    m_parameters.push_back(pointer);
  }

  /**Adds a required parameter.*/
  template <typename T>
  void addRequiredParameter(std::string name,
                            std::string description,
                            std::vector<ParameterCheckPtr> checks = {})
  {
    if (this->hasParameter(name))
      throw std::runtime_error(m_name + ": Trying to add parameter \"" + name +
                               "\", but the parameter already exists.");

    T default_value;
    auto pointer = makeTemplatedInputParameter(
      name, ParameterClass::REQUIRED, default_value, description, checks);

    m_parameters.push_back(pointer);
  }

  /**Checks whether the parameter with the given name is present.*/
  bool hasParameter(const std::string& name) const
  {
    for (const auto& parameter_ptr : m_parameters) // NOLINT(*-use-anyofallof)
      if (parameter_ptr->name() == name) return true;

    return false;
  }

  /**Obtains a parameter name.*/
  const InputParameter& getParameter(const std::string& name) const;

  /**Obtains the value of a named parameter.*/
  template <typename T>
  T getParameterValue(const std::string& name) const
  {
    const auto& param = getParameter(name);

    return param.getValue<T>();
  }

  /**Iterates over parameters.*/
  class iterator
  {
    InputParametersBlock& m_reference_block;
    size_t m_index;

  public:
    iterator(InputParametersBlock& reference_block, const size_t index)
      : m_reference_block(reference_block), m_index(index)
    {
    }

    // clang-format off
    iterator operator++() { const iterator i = *this; m_index++; return i; }
    iterator operator++(int) { m_index++; return *this; }

    InputParameter& operator*() const { return *m_reference_block.m_parameters[m_index]; }
    bool operator==(const iterator& rhs) const { return m_index == rhs.m_index; }
    bool operator!=(const iterator& rhs) const { return m_index != rhs.m_index; }
    // clang-format on
  };

  /**Iterates over parameters of a constant block.*/
  class const_iterator
  {
    const InputParametersBlock& m_reference_block;
    size_t m_index;

  public:
    const_iterator(const InputParametersBlock& reference_block,
                   const size_t index)
      : m_reference_block(reference_block), m_index(index)
    {
    }

    // clang-format off
    const_iterator operator++() { const const_iterator i = *this; m_index++; return i; }
    const_iterator operator++(int) { m_index++; return *this; }

    const InputParameter& operator*() const { return *m_reference_block.m_parameters[m_index]; }
    bool operator==(const const_iterator& rhs) const { return m_index == rhs.m_index; }
    bool operator!=(const const_iterator& rhs) const { return m_index != rhs.m_index; }
    // clang-format on
  };

  iterator begin() { return {*this, 0}; }

  iterator end() { return {*this, m_parameters.size()}; }

  const_iterator begin() const { return {*this, 0}; }

  const_iterator end() const { return {*this, m_parameters.size()}; }

  size_t size() const { return m_parameters.size(); }

private:
  /**Parameter maker for Scalar Input Parameters.*/
  template <typename T>
  std::enable_if_t<IsScalar<T>::value, InputParameterPtr>
  makeTemplatedInputParameter(std::string name,
                             ParameterClass param_class,
                             T default_value,
                             std::string description,
                             std::vector<ParameterCheckPtr> checks)
  {
    if (checks.empty())
      checks = {std::make_shared<param_checks::ScalarTypeMustMatch>()};

    auto pointer = std::make_shared<ScalarInputParameter<T>>(
      name, param_class, default_value, description, checks);

    return pointer;
  }

  /**Parameter maker for Array Input Parameters.*/
  template <typename T>
  std::enable_if_t<IsVector<T>::value, InputParameterPtr>
  makeTemplatedInputParameter(std::string name,
                             ParameterClass param_class,
                             T default_value,
                             std::string description,
                             std::vector<ParameterCheckPtr> checks)
  {
    if (checks.empty())
      checks = {std::make_shared<param_checks::ScalarTypeMustMatch>()};

    auto pointer = std::make_shared<SequenceInputParameter<T>>(
      name, param_class, default_value, description, checks);

    return pointer;
  }
};

} // namespace elke

#endif // ELK_E_INPUTPARAMETERS_H
