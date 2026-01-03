#ifndef ELK_E_INPUTPARAMETERS_H
#define ELK_E_INPUTPARAMETERS_H

#include "InputParameter.h"
#include "ParameterCheck.h"
#include "elke_core/utilities/general_utils.h"
#include "elke_core/utilities/special_iterators.h"
#include "parameter_types/ScalarInputParameter.h"
#include "parameter_types/VectorOfScalarsInputParameter.h"
#include "parameter_types/RegisteredObjectInputParameter.h"
#include "parameter_types/VectorOfRegisteredObjectsInputParameter.h"
#include "parameter_types/MapOfRegisteredObjectsInputParameter.h"

#include <vector>

namespace elke
{

using InputParameterPtr = std::unique_ptr<InputParameter>;

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

  /**Copy constructor.*/
  InputParametersBlock(const InputParametersBlock& rhs)
    : m_name(rhs.m_name),
      m_block_description(rhs.m_block_description),
      m_parameters(elke::copyVectorOfUniquePointers(rhs.m_parameters))
  {
  }

  /**Returns the name of the input block.*/
  const std::string& name() const { return m_name; }

  /**Adds an optional parameter.*/
  template <typename T>
  void addOptionalParameter(std::string name,
                            T default_value,
                            std::string description,
                            const std::vector<ParameterCheckPtr>& checks = {})
  {
    this->addParameter(
      ParameterClass::OPTIONAL, name, default_value, description, checks);
  }

  /**Adds a required parameter.*/
  template <typename T>
  void addRequiredParameter(const std::string name,
                            const std::string description,
                            const std::vector<ParameterCheckPtr>& checks = {})
  {
    this->addParameter<T>(ParameterClass::REQUIRED,
                          name,
                          /*default_value=*/T(),
                          description,
                          checks);
  }

  void addParameterAsNamedInputBlock(const std::string& name,
                                     const std::string& description,
                                     ParameterClass parameter_class,
                                     const std::string& name_of_block);
  void addParameterAsArrayOfNamedInputBlocks(const std::string& name,
                                             const std::string& description,
                                             ParameterClass parameter_class,
                                             const std::string& name_of_blocks);
  void addParameterAsFixedArrayOfNamedInputBlocks(
    const std::string& name,
    const std::string& description,
    ParameterClass parameter_class,
    const std::vector<std::string>& name_of_blocks);
  void addParameterAsMapOfNamedInputBlocks(const std::string& name,
                                           const std::string& description,
                                           ParameterClass parameter_class,
                                           const std::string& name_of_blocks);

  // clang-format off
  /**Returns an iterable container for the parameters in this block.*/
  auto iterableParameters() { return VectorOfPointersIterable(m_parameters); }
  /**Returns a const-iterable container for the parameters in this block.*/
  auto iterableParameters() const { return VectorOfPointersConstIterable(m_parameters);}
  // clang-format on

  /**Checks whether the parameter with the given name is present.*/
  bool hasParameter(const std::string& name) const;

  /**Obtains a parameter by name.*/
  const InputParameter& getParameter(const std::string& name) const;

  /**Obtains the value of a named parameter.*/
  template <typename T>
  T getParameterValue(const std::string& name) const
  {
    const auto& param = getParameter(name);

    return param.getValue<T>();
  }

  void checkInputDataValidity(const DataTree& data,
                              WarningsAndErrorsData& warnings_and_errors_data,
                              unsigned int nest_depth) const;

private:
  /**Templated method to add a parameter*/
  template <typename T>
  void addParameter(const ParameterClass parameter_class,
                    const std::string name,
                    T default_value,
                    const std::string description,
                    const std::vector<ParameterCheckPtr> checks)
  {
    if (this->hasParameter(name))
      throw std::runtime_error(m_name + ": Trying to add parameter \"" + name +
                               "\", but the parameter already exists.");

    auto meta_data =
      InputParameterMetaData{name, parameter_class, description, checks};
    using namespace param_checks;
    using std::make_unique;

    auto new_param = makeParameter(meta_data, default_value);

    m_parameters.push_back(std::move(new_param));
  }

  // clang-format off
  /**Make a scalar input parameter.*/
  template <typename T, std::enable_if_t<IsScalar<T>::value, bool> = true>
  std::unique_ptr<InputParameter> makeParameter(const InputParameterMetaData& meta_data, T default_value)
  {
    return std::make_unique<ScalarInputParameter>(meta_data, default_value);
  }

  /**Make an array of scalars parameter.*/
  template <typename T, std::enable_if_t<IsVectorOfScalars<T>::value, bool> = true>
  std::unique_ptr<InputParameter> makeParameter(const InputParameterMetaData& meta_data, T default_value)
  {
    return std::make_unique<VectorOfScalarsInputParameter>(meta_data, default_value);
  }

  /**Make a parameter used to create a registered object.*/
  template <typename T, std::enable_if_t<IsRegisteredObjectProxy<T>::value, bool> = true>
  std::unique_ptr<InputParameter> makeParameter(const InputParameterMetaData& meta_data, T default_value)
  {
    return std::make_unique<RegisteredObjectInputParameter>(meta_data, default_value);
  }

  /**Make a parameter used to create an array of registered objects.*/
  template <typename T, std::enable_if_t<IsRegisteredObjectProxyArray<T>::value, bool> = true>
  std::unique_ptr<InputParameter> makeParameter(const InputParameterMetaData& meta_data, T default_value)
  {
    return std::make_unique<VectorOfRegisteredObjectsInputParameter>(meta_data, default_value);
  }

  /**Make a parameter used to create a map of registered objects.*/
  template <typename T, std::enable_if_t<IsRegisteredObjectProxyMap<T>::value, bool> = true>
  std::unique_ptr<InputParameter> makeParameter(const InputParameterMetaData& meta_data, T default_value)
  {
    return std::make_unique<MapOfRegisteredObjectsInputParameter>(meta_data, default_value);
  }
  // clang-format on

  /**Clones and adds a parameter to the block.*/
  void cloneAndAddParameter(const InputParameter& parameter)
  {
    m_parameters.push_back(parameter.clone());
  }
};

} // namespace elke

#endif // ELK_E_INPUTPARAMETERS_H
