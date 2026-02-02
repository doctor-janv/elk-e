#include "ParameterTree.h"

#include "elke_core/output/elk_exceptions.h"
#include "elke_core/utilities/string_utils.h"

#include <unordered_set>
#include <sstream>
#include <iostream>
#include <utility>

namespace elke
{

// ###################################################################
/**Generates the string equivalent of a parameter label.*/
// ReSharper disable once CppNotAllPathsReturnValue
std::string ParameterLabelToString(const ParameterLabel label)
{
  switch (label)
  {
      // clang-format off
    case ParameterLabel::OPTIONAL:   return "OPTIONAL";
    case ParameterLabel::REQUIRED:   return "REQUIRED";
    case ParameterLabel::DEPRECATED: return "DEPRECATED";
      // clang-format on
  }
}

/**Empty block creation. Can only be of map type.*/
ParameterTree::ParameterTree(std::string name, std::string description)
  : m_name(std::move(name)),
    m_description(std::move(description)),
    m_label(ParameterLabel::OPTIONAL),
    m_gross_type(DataGrossType::MAP),
    m_parent_tree_ptr(nullptr),
    m_is_a_specification_map(true)
{
}

/**Compound assignment operator.*/
ParameterTree& ParameterTree::operator+=(const ParameterTree& other)
{
  // for (const auto& child : other.constIterableParameters())
  // {
  //   assertAndThrowIfDuplicate(child.name());
  //   m_children.push_back(child);
  // }

  return *this;
}

/**Returns the name of the tree.*/
const std::string& ParameterTree::name() const { return m_name; }
/**Returns the label of the tree.*/
ParameterLabel ParameterTree::label() const { return m_label; }
/**Sets the assignment flag for the particular tree.*/
void ParameterTree::setAssignmentFlag(const bool value)
{
  m_assignment_flag = value;
}

// ###################################################################
/**Adds additional input checks.*/
void ParameterTree::addAdditionalInputCheck(
  std::unique_ptr<param_checks::InputCheck> check_ptr,
  bool allow_subsequent_checks /*=true*/)
{
  m_additional_input_checks.emplace_back(std::move(check_ptr),
                                         allow_subsequent_checks);
}

// ###################################################################
/**Perform all additional checks.*/
bool ParameterTree::performAdditionalChecks(StatusStrings& status_strings,
                                            const DataTree& data,
                                            const std::string& prefix,
                                            const std::string& name_or_id) const
{
  bool passed = true;
  for (const auto& check : m_additional_input_checks)
  {
    const auto& check_obj = check.m_check;
    const bool check_passed =
      check_obj->performCheck(status_strings, data, prefix, name_or_id);

    if (not check_passed and not check.m_allow_subsequent_checks)
    {
      status_strings.m_errors += prefix + "Additional checks skipped.\n";
      return false;
    }

    if (not check_passed) passed = false;
  }

  return passed;
}

// ###################################################################
/**Checks whether the parameter with the given name is present.*/
bool ParameterTree::hasParameter(const std::string& name) const
{
  // clang-format off
  for (const auto& parameter : this->constIterableParameters()) // NOLINT(*-use-anyofallof)
    if (parameter.name() == name) return true;

  return false;
  // clang-format on
}

// ###################################################################
/**Obtains const-reference to a parameter by name.*/
const ParameterTree& ParameterTree::getParameter(const std::string& name) const
{
  for (const auto& parameter : this->constIterableParameters())
    if (parameter.name() == name) return parameter;

  elkLogicalError("ParameterTree has no parameter named \"" + name + "\"");
}

// ###################################################################
/**Obtains a non-const-reference to a parameter by name.*/
ParameterTree& ParameterTree::getParameter(const std::string& name)
{
  for (auto& parameter : this->iterableParameters())
    if (parameter.name() == name) return parameter;

  elkLogicalError("ParameterTree has no parameter named \"" + name + "\"");
}

// ###################################################################
void ParameterTree::checkAndAssignData(StatusStrings& status_strings,
                                       const DataTree& data)
{

  // std::cout << "Master checkAndAssignData " << this->name() << " "
  //           << DataGrossTypeName(m_gross_type) << " " << data.name() << " "
  //           << DataGrossTypeName(data.grossType()) << "\n"; // TODO: Remove
  m_assignment_flag = getRecursiveParentAssignmentFlag();
  m_nest_depth = getRecursiveNestDepth();

  if (not grossTypeMatches(status_strings.m_errors, data)) return;

  // clang-format off
  switch (m_gross_type)
  {
    case DataGrossType::SCALAR: checkAndAssignScalarData(status_strings, data); break;
    case DataGrossType::SEQUENCE: checkAndAssignSequenceData(status_strings, data); break;
    case DataGrossType::MAP:
      if (m_is_a_specification_map) processSpecification(status_strings, data);
      else checkAndAssignArbitraryMap(status_strings, data);
    default: break;
  }
  // clang-format on
}

// ###################################################################
void ParameterTree::checkAndAssignScalarData(StatusStrings& status_strings,
                                             const DataTree& data)
{
  // std::cout << "checkAndAssignScalarData " << m_nest_depth << "\n";
  const auto indent = std::string((m_nest_depth + 1) * 2, ' ');

  const auto target_type = m_meta_data.m_scalar_options.m_scalar_type;
  const auto& data_scalar_value = data.value();
  const auto& options = m_meta_data.m_scalar_options;

  switch (options.m_scalar_assignment)
  {
    case param_options::ScalarAssignment::MUST_MATCH_EXACTLY:
      if (data_scalar_value.type() != target_type)
      {
        std::stringstream error_message;
        // clang-format off
        error_message
          << indent << "Item \"" << this->name() << "\" is required to be of specific scalar-type "
          << scalarTypeStringName(target_type) << ". Supplied scalar-type is "
          << scalarTypeStringName(data_scalar_value.type()) << " with value "
          << data_scalar_value.convertToString() << ".\n";
        // clang-format on

        status_strings.m_errors.append(error_message.str());
        return;
      }
    case param_options::ScalarAssignment::MUST_BE_COMPATIBLE:
      if (not data_scalar_value.isConvertibleToType(target_type))
      {
        std::stringstream error_message;
        // clang-format off
        error_message
          << indent << "Item \"" << this->name() << "\" is required to be of scalar-type "
          << scalarTypeStringName(target_type) << ". Supplied scalar-type is "
          << scalarTypeStringName(data_scalar_value.type()) << " with value "
          << data_scalar_value.convertToString() << " which is not compatible with "
          << "scalar-type " << scalarTypeStringName(target_type)
          << ".\n";
        // clang-format on

        status_strings.m_errors.append(error_message.str());
        return;
      }
    case param_options::ScalarAssignment::CAN_BE_ANYTHING:
      break;
  }

  const bool checks_passed =
    performAdditionalChecks(status_strings, data, indent, this->name());

  if (m_assignment_flag == true and checks_passed)
    m_assigned_data_tree = std::make_shared<DataTree>(data);
}

// ###################################################################
void ParameterTree::checkAndAssignSequenceData(StatusStrings& status_strings,
                                               const DataTree& data)
{
  // clang-format off
  switch (m_meta_data.m_array_options.m_nature)
  {
    case param_options::ArrayNature::SCALARS:
      checkAndAssignArrayOfScalars(status_strings, data); break;
    case param_options::ArrayNature::ARBITRARY:
      checkAndAssignArrayOfArbs(status_strings, data); break;
    default: break;
  }
  // clang-format on
}

// ###################################################################
void ParameterTree::checkAndAssignArrayOfScalars(StatusStrings& status_strings,
                                                 const DataTree& data)
{
  const auto indent = std::string((m_nest_depth + 1) * 2, ' ');

  size_t id = 0;
  for (const auto& child_ptr : data.constChildren())
  {
    const auto child_gross_type = child_ptr->grossType();
    if (child_gross_type != DataGrossType::SCALAR)
    {
      std::stringstream error_message;
      // clang-format off
      error_message
        << indent << "Array entry " << id << " is required to be of gross-type SCALAR. Supplied gross-type is "
        << DataGrossTypeName(child_gross_type) << ".\n";
      // clang-format on
      status_strings.m_errors.append(error_message.str());
      ++id;
      continue;
    }

    const auto& data_scalar_value = child_ptr->value();

    auto& options = m_meta_data.m_array_options.m_array_of_scalar_options;
    const auto target_type = options.m_scalar_type;

    switch (options.m_scalar_assignment)
    {
      case param_options::ScalarAssignment::MUST_MATCH_EXACTLY:
        if (data_scalar_value.type() != target_type)
        {
          std::stringstream error_message;
          // clang-format off
          error_message
            << indent << "Array entry " << id << " is required to be of specific scalar-type "
            << scalarTypeStringName(target_type) << ". Supplied scalar-type is "
            << scalarTypeStringName(data_scalar_value.type()) << " with value "
            << data_scalar_value.convertToString() << ".\n";
          // clang-format on

          status_strings.m_errors.append(error_message.str());
          ++id;
          continue;
        }
      case param_options::ScalarAssignment::MUST_BE_COMPATIBLE:
        if (not data_scalar_value.isConvertibleToType(target_type))
        {
          std::stringstream error_message;
          // clang-format off
          error_message
            << indent << "Item \"" << this->name() << "\" is required to be of scalar-type "
            << scalarTypeStringName(target_type) << ". Supplied scalar-type is "
            << scalarTypeStringName(data_scalar_value.type()) << " with value "
            << data_scalar_value.convertToString() << " which is not compatible with "
            << "scalar-type " << scalarTypeStringName(target_type)
            << ".\n";
          // clang-format on

          status_strings.m_errors.append(error_message.str());
          ++id;
          continue;
        }
      case param_options::ScalarAssignment::CAN_BE_ANYTHING:
        break;
    }

    ++id;
  } // for (const auto& child_ptr

  const bool checks_passed =
    performAdditionalChecks(status_strings, data, indent, this->name());

  if (m_assignment_flag == true and checks_passed)
    m_assigned_data_tree = std::make_shared<DataTree>(data);
}

void ParameterTree::checkAndAssignArrayOfArbs(StatusStrings& status_strings,
                                              const DataTree& data)
{
  const auto indent = std::string((m_nest_depth + 1) * 2, ' ');
  // Gross-type already checked

  const bool checks_passed =
    performAdditionalChecks(status_strings, data, indent, this->name());

  if (m_assignment_flag == true and checks_passed)
    m_assigned_data_tree = std::make_shared<DataTree>(data);
}

// ###################################################################
void ParameterTree::checkAndAssignArbitraryMap(StatusStrings& status_strings,
                                               const DataTree& data)
{
  const auto indent = std::string((m_nest_depth + 1) * 2, ' ');
  // Gross-type already checked

  const bool checks_passed =
    performAdditionalChecks(status_strings, data, indent, this->name());

  if (m_assignment_flag == true and checks_passed)
    m_assigned_data_tree = std::make_shared<DataTree>(data);
}

// ###################################################################
void ParameterTree::processSpecification(StatusStrings& output_status_strings,
                                         const DataTree& data)
{
  // std::cout << "checkAndAssignMapData " << this->name() << " " << data.name()
  //           << "\n";
  const auto indent = std::string((m_nest_depth + 1) * 2, ' ');

  StatusStrings this_status_strings;

  //=================================== First we check if the data is actually
  //                                    valid, if not we mark them to be skipped
  //                                    because we provide an error for them
  //                                    once
  std::vector<std::string> invalid_param_names;
  std::vector<DataTreeConstPtr> valid_data_children;

  for (const auto& child_ptr : data.constChildren())
    if (not this->hasParameter(child_ptr->name()))
      invalid_param_names.emplace_back(child_ptr->name());
    else
      valid_data_children.emplace_back(child_ptr);

  //=================================== For each of the invalid parameters
  //                                    check if we have a suggestion.
  if (not invalid_param_names.empty())
  {
    const auto valid_param_names = this->validParameterNames();

    for (const auto& invalid_param_name : invalid_param_names)
    {
      const auto suggestion = string_utils::findClosestMatchingString(
        invalid_param_name, valid_param_names);

      const std::string suggestion_str =
        suggestion.empty()
          ? " No suggested parameter name could be determined.\n"
          : " Did you mean \"" + suggestion + "\"?\n";

      this_status_strings.m_errors.append(indent + "The parameter name \"" +
                                          invalid_param_name +
                                          "\" is invalid." + suggestion_str);
    } // for invalid_param_name
  } // if (not invalid_param_names.empty())

  //=================================== Now we check if required parameters
  //                                    have been supplied
  for (const auto& parameter : this->iterableParameters())
  {
    const bool is_required = parameter.label() == ParameterLabel::REQUIRED;
    const bool is_deprecated = parameter.label() == ParameterLabel::DEPRECATED;
    const bool in_data_list = data.hasChild(parameter.name());
    if (is_required and not in_data_list)
      this_status_strings.m_errors.append(indent + "Required parameter \"" +
                                          parameter.name() +
                                          "\" not supplied.\n");
    if (is_deprecated and in_data_list)
      this_status_strings.m_warnings.append(
        indent + "Parameter \"" + parameter.name() + "\" is deprecated.");
  }

  //=================================== Now for each data entry, if not skipped,
  //                                    check whether the value is valid.
  StatusStrings child_status_strings;
  for (const auto& child_ptr : valid_data_children)
  {
    const auto& child = *child_ptr;

    auto& input_parameter = this->getParameter(child.name());

    //***********************
    // Individual parameter check
    input_parameter.checkAndAssignData(child_status_strings, child);
    //***********************
  } // for child
  this_status_strings += child_status_strings;

  //=================================== Format final error strings
  // clang-format off
  const std::string context =
    "While checking input parameter validity for block "
    "\"" + this->name() + "\" from data-tree \"" + data.getTag("address") + "\""
    " parsed from " + data.getTag("mark") + ":\n";
  // clang-format on

  auto& output = output_status_strings;
  if (not this_status_strings.m_errors.empty())
    output.m_errors.append(context + this_status_strings.m_errors);
  if (not this_status_strings.m_warnings.empty())
    output.m_warnings.append(context + this_status_strings.m_warnings);

  const bool checks_passed =
    performAdditionalChecks(this_status_strings, data, indent, this->name());

  if (m_assignment_flag == true and checks_passed)
    m_assigned_data_tree = std::make_shared<DataTree>(data);
  m_assigned_data_tree = std::make_shared<DataTree>(data);
}

// ###################################################################
/**Creates a sorted list of all the children names.*/
std::vector<std::string> ParameterTree::validParameterNames() const
{
  std::unordered_set<std::string> valid_param_names_set;
  for (const auto& child : this->constIterableParameters())
    valid_param_names_set.insert(child.name());

  std::vector<std::string> vec_of_names;
  vec_of_names.reserve(valid_param_names_set.size());
  for (const auto& name : valid_param_names_set)
    vec_of_names.emplace_back(name);

  return vec_of_names;
}

// ###################################################################
/**If the new parameter name is already in the list of children, a
 * std::logic_error is thrown. */
void ParameterTree::assertAndThrowIfDuplicate(
  const std::string& new_parameter_name) const
{
  for (const auto& child_ptr : m_children)
    if (child_ptr->name() == new_parameter_name)
      elkLogicalError("Parameter named \"" + new_parameter_name +
                      "\" already in tree named \"" + this->name() + "\".");
}

// ##################################################################
/**Checks if gross type matches.*/
bool ParameterTree::grossTypeMatches(std::string& error_string,
                                     const DataTree& data) const
{
  if (m_gross_type == data.grossType()) return true;

  const auto indent = std::string((m_nest_depth + 1) * 2, ' ');

  std::stringstream error_message;
  // clang-format off
  error_message
    << indent << "Item \"" << this->name()
    << "\" is required to be of gross-type "
    << DataGrossTypeName(m_gross_type) << ". Supplied gross-type is "
    << DataGrossTypeName(data.grossType()) << ".\n";
  // clang-format on

  error_string.append(error_message.str());

  return false;
}

// ##################################################################
/**Get assignment flag. This is recursive on the parent of this tree.*/
bool ParameterTree::getRecursiveParentAssignmentFlag() const
{
  if (m_parent_tree_ptr == nullptr) return m_assignment_flag;

  return m_parent_tree_ptr->getRecursiveParentAssignmentFlag();
}

// ##################################################################
/**Recursively determines the current nesting depth.*/
size_t ParameterTree::getRecursiveNestDepth() const
{
  if (m_parent_tree_ptr == nullptr) return 0;

  return m_parent_tree_ptr->getRecursiveNestDepth() + 1;
}

} // namespace elke
