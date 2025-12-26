#ifndef ELK_E_INPUTPARAMETERS_H
#define ELK_E_INPUTPARAMETERS_H

#include "InputParameter.h"
#include "ParameterCheck.h"
#include "elke_core/utilities/general_utils.h"

#include <vector>

namespace elke
{

using InputParameterPtr = std::shared_ptr<InputParameter>;

class InputParametersBlock
{
  const std::string m_name;
  const std::string m_block_description;
  std::vector<InputParameter> m_parameters;

public:
  /**Creates a barebones empty block.*/
  InputParametersBlock(
    std::string name,
    std::string description,
    const std::vector<InputParametersBlock>& parent_blocks = {});

  /**Returns the name of the input block.*/
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

    if (checks.empty() and IsScalar<T>::value)
      checks = {std::make_unique<param_checks::ScalarTypeMustBeCompatible>()};

    if (checks.empty() and IsVectorOfScalars<T>::value)
      checks = {
        std::make_unique<param_checks::ScalarArrayEntriesTypeMustBeCompatible>()};

    if (checks.empty() and IsRegisteredObjectProxy<T>::value)
      checks = {std::make_unique<param_checks::RegisteredObjectMustExist>()};

    if (checks.empty() and IsRegisteredObjectProxyArray<T>::value)
      checks = {std::make_unique<
        param_checks::RegisteredObjectArrayEntriesMustExist>()};

    m_parameters.emplace_back(
      name, ParameterClass::OPTIONAL, default_value, description, checks);
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

    if (checks.empty() and IsScalar<T>::value)
      checks = {std::make_unique<param_checks::ScalarTypeMustBeCompatible>()};

    if (checks.empty() and IsVectorOfScalars<T>::value)
      checks = {
        std::make_unique<param_checks::ScalarArrayEntriesTypeMustBeCompatible>()};

    if (checks.empty() and IsRegisteredObjectProxy<T>::value)
      checks = {std::make_unique<param_checks::RegisteredObjectMustExist>()};

    if (checks.empty() and IsRegisteredObjectProxyArray<T>::value)
      checks = {std::make_unique<
        param_checks::RegisteredObjectArrayEntriesMustExist>()};

    T default_value;
    m_parameters.emplace_back(
      name, ParameterClass::REQUIRED, default_value, description, checks);
  }

  /**Checks whether the parameter with the given name is present.*/
  bool hasParameter(const std::string& name) const
  {
    for (const auto& parameter : m_parameters) // NOLINT(*-use-anyofallof)
      if (parameter.name() == name) return true;

    return false;
  }

  /**Obtains a parameter by name.*/
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

    InputParameter& operator*() const { return m_reference_block.m_parameters[m_index]; }
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

    const InputParameter& operator*() const { return m_reference_block.m_parameters[m_index]; }
    bool operator==(const const_iterator& rhs) const { return m_index == rhs.m_index; }
    bool operator!=(const const_iterator& rhs) const { return m_index != rhs.m_index; }
    // clang-format on
  };

  iterator begin() { return {*this, 0}; }

  iterator end() { return {*this, m_parameters.size()}; }

  const_iterator begin() const { return {*this, 0}; }

  const_iterator end() const { return {*this, m_parameters.size()}; }

  size_t size() const { return m_parameters.size(); }

  void checkInputDataValidity(const DataTree& data,
                              WarningsAndErrorsData& warnings_and_errors_data,
                              unsigned int nest_depth) const;
};

} // namespace elke

#endif // ELK_E_INPUTPARAMETERS_H
