#include "InputParametersBlock.h"

#include "elke_core/output/elk_exceptions.h"
#include "elke_core/utilities/string_utils.h"
#include "parameter_types/FixedVectorOfNamedBlocksInputParameter.h"
#include "parameter_types/NamedBlockInputParameter.h"
#include "parameter_types/VectorOfNamedBlocksInputParameter.h"
#include "parameter_types/MapOfNamedBlocksInputParameter.h"

#include <utility>
#include <unordered_set>
#include <sstream>

namespace elke
{

// ###################################################################
/**Creates a barebones empty block.*/
InputParametersBlock::InputParametersBlock(
  std::string name,
  std::string description,
  const std::vector<InputParametersBlock>& parent_blocks /*={}*/)
  : m_name(std::move(name)), m_block_description(std::move(description))
{
  for (const auto& parent_block : parent_blocks)
    for (const auto& parameter : parent_block.iterableParameters())
      if (not this->hasParameter(parameter.name()))
        this->cloneAndAddParameter(parameter);
      else
      {
        throw std::runtime_error(
          "While adding parameters from parent block \"" + parent_block.name() +
          "\" to block \"" + name + "\", parameter \"" + parameter.name() +
          "\" is already added and cannot be duplicated.");
      }
}

// ###################################################################
/**Obtains a parameter by name.*/
const InputParameter&
InputParametersBlock::getParameter(const std::string& name) const
{
  for (const auto& parameter : this->iterableParameters())
    if (parameter.name() == name) return parameter;

  elkLogicalError("InputParameterBlock has no parameter named \"" + name +
                  "\"");
}

// ###################################################################
/**Checks whether the parameter with the given name is present.*/
bool InputParametersBlock::hasParameter(const std::string& name) const
{
  // clang-format off
  for (const auto& parameter : this->iterableParameters()) // NOLINT(*-use-anyofallof)
    if (parameter.name() == name) return true;

  return false;
  // clang-format on
}

// ###################################################################
void InputParametersBlock::checkInputDataValidity(
  const DataTree& data,
  WarningsAndErrorsData& warnings_and_errors_data,
  const unsigned int nest_depth) const
{
  const auto indent = std::string((nest_depth + 1) * 2, ' ');
  const std::string context = "While checking input parameter validity for "
                              "block \"" +
                              this->name() + "\" from data-tree \"" +
                              data.getTag("address") + "\":\n";

  // auto& errors = warnings_and_errors_data.m_errors;
  // auto& warnings = warnings_and_errors_data.m_warnings;
  std::vector<std::string> errors;
  std::vector<std::string> warnings;

  //=================================== First we check if the data is actually
  //                                    valid, if not we mark them to be skipped
  //                                    because we provide an error for them
  //                                    once
  std::vector<std::string> invalid_param_names;

  for (const auto& child : data.children())
    if (not this->hasParameter(child->name()))
      invalid_param_names.emplace_back(child->name());

  //=================================== For each of the invalid parameters
  //                                    check if we have a suggestion.
  if (not invalid_param_names.empty())
  {
    std::unordered_set<std::string> valid_param_names_set;
    for (const auto& child : this->iterableParameters())
      valid_param_names_set.insert(child.name());

    for (const auto& invalid_param_name : invalid_param_names)
    {
      const auto suggestion = string_utils::findClosestMatchingString(
        invalid_param_name, valid_param_names_set);

      const std::string suggestion_str =
        suggestion.empty()
          ? " No suggested parameter name could be determined.\n"
          : " Did you mean \"" + suggestion + "\"?\n";

      errors.emplace_back(
        indent +
        "The parameter name \"" + // NOLINT(*-inefficient-string-concatenation)
        invalid_param_name +
        "\" is invalid." + // NOLINT(*-inefficient-string-concatenation)
        suggestion_str);
    }
  } // if (not invalid_param_names.empty())

  //=================================== Now for each data entry, if not skipped,
  //                                    check whether the value is valid.
  for (const auto& child_ptr : data.children())
  {
    const auto& child = *child_ptr;
    if (string_utils::stringListHasString(invalid_param_names, child.name()))
      continue;

    const auto& input_parameter = this->getParameter(child.name());

    //***********************
    // Individual parameter check
    auto result = input_parameter.performChecks(child, nest_depth);
    //***********************

    if (not result.m_error.empty()) errors.emplace_back(result.m_error);
  } // for child

  //=================================== Now we check if required parameters
  //                                    have been supplied
  for (const auto& parameter : this->iterableParameters())
    if (parameter.classType() == ParameterClass::REQUIRED and
        not data.hasChild(parameter.name()))
      errors.emplace_back(indent + "Required parameter \"" + parameter.name() +
                          "\" not supplied.\n");

  //=================================== Finally we check if deprecated
  //                                    parameters have been used, and issue
  //                                    warnings if they were
  for (const auto& parameter : this->iterableParameters())
    if (parameter.classType() == ParameterClass::DEPRECATED and
        data.hasChild(parameter.name()))
      warnings.emplace_back(indent + "Parameter \"" + parameter.name() +
                            "\" is deprecated.");

  auto joinStrings = [&context](const std::vector<std::string>& input)
  {
    std::stringstream output;
    if (not input.empty()) output << context;
    for (const auto& str : input)
      output << str;

    return output.str();
  };

  if (not errors.empty())
    warnings_and_errors_data.m_errors.emplace_back(joinStrings(errors));
  if (not warnings.empty())
    warnings_and_errors_data.m_warnings.emplace_back(joinStrings(warnings));
}

// ###################################################################
void InputParametersBlock::addParameterAsNamedInputBlock(
  const std::string& name,
  const std::string& description,
  const ParameterClass parameter_class,
  const std::string& name_of_block)
{
  if (this->hasParameter(name))
    throw std::runtime_error(m_name + ": Trying to add parameter \"" + name +
                             "\", but the parameter already exists.");
  auto meta_data = InputParameterMetaData{name, parameter_class, description};

  auto new_param =
    std::make_unique<NamedBlockInputParameter>(meta_data, name_of_block);

  m_parameters.push_back(std::move(new_param));
}

// ###################################################################
void InputParametersBlock::addParameterAsArrayOfNamedInputBlocks(
  const std::string& name,
  const std::string& description,
  const ParameterClass parameter_class,
  const std::string& name_of_blocks)
{
  if (this->hasParameter(name))
    throw std::runtime_error(m_name + ": Trying to add parameter \"" + name +
                             "\", but the parameter already exists.");

  auto meta_data = InputParameterMetaData{name, parameter_class, description};

  auto new_param = std::make_unique<VectorOfNamedBlocksInputParameter>(
    meta_data, name_of_blocks);

  m_parameters.push_back(std::move(new_param));
}

// ###################################################################
void InputParametersBlock::addParameterAsFixedArrayOfNamedInputBlocks(
  const std::string& name,
  const std::string& description,
  const ParameterClass parameter_class,
  const std::vector<std::string>& name_of_blocks)
{
  if (this->hasParameter(name))
    throw std::runtime_error(m_name + ": Trying to add parameter \"" + name +
                             "\", but the parameter already exists.");

  auto meta_data = InputParameterMetaData{name, parameter_class, description};

  auto new_param = std::make_unique<FixedVectorOfNamedBlocksInputParameter>(
    meta_data, name_of_blocks);

  m_parameters.push_back(std::move(new_param));
}

// ###################################################################
void InputParametersBlock::addParameterAsMapOfNamedInputBlocks(
  const std::string& name,
  const std::string& description,
  const ParameterClass parameter_class,
  const std::string& name_of_blocks)
{
  if (this->hasParameter(name))
    throw std::runtime_error(m_name + ": Trying to add parameter \"" + name +
                             "\", but the parameter already exists.");

  auto meta_data = InputParameterMetaData{name, parameter_class, description};

  auto new_param =
    std::make_unique<MapOfNamedBlocksInputParameter>(meta_data, name_of_blocks);

  m_parameters.push_back(std::move(new_param));
}

} // namespace elke