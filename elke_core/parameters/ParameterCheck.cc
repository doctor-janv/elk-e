#include "ParameterCheck.h"
#include "elke_core/data_types/DataGrossType.h"
#include "InputParameter.h"
#include "elke_core/input/InputProcessor.h"
#include "elke_core/registration/registration.h"
#include "parameter_types/NamedBlockInputParameter.h"
#include "parameter_types/VectorOfNamedBlocksInputParameter.h"
#include "parameter_types/FixedVectorOfNamedBlocksInputParameter.h"
#include "parameter_types/MapOfNamedBlocksInputParameter.h"

namespace elke::param_checks
{
// ###################################################################
ParameterCheckResult
GrossTypeMustMatch::checkParameter(const DataTree& data,
                                   const InputParameter& parameter,
                                   const unsigned int nest_depth)
{
  const auto indent = std::string((nest_depth + 1) * 2, ' ');
  ParameterCheckResult result;

  if (data.grossType() != parameter.grossType())
  {
    result.m_error +=
      indent + "Item \"" + data.name() + "\" is required to be of type " +
      DataGrossTypeName(parameter.grossType()) + ". Supplied type is " +
      DataGrossTypeName(data.grossType()) + ".\n";
  }

  return result;
}

// ###################################################################
ParameterCheckResult
ScalarTypeMustMatch::checkParameter(const DataTree& data,
                                    const InputParameter& parameter,
                                    const unsigned int nest_depth)
{
  const auto indent = std::string((nest_depth + 1) * 2, ' ');
  auto check_result = m_gross_check.checkParameter(data, parameter, nest_depth);

  if (not check_result.m_error.empty()) return check_result;

  const auto& data_value = data.value();
  const auto parameter_scalar_type = parameter.scalarType();

  if (data_value.type() != parameter_scalar_type)
  {
    check_result.m_error +=
      indent + std::string("ScalarTypeMustMatch: ") + "Item \"" + data.name() +
      "\" is required to be of type " +
      varyingDataTypeStringName(parameter_scalar_type) + ". Supplied type is " +
      data_value.typeName() + ".\n";
  }

  return check_result;
}

// ###################################################################
ParameterCheckResult
ScalarTypeMustBeCompatible::checkParameter(const DataTree& data,
                                           const InputParameter& parameter,
                                           const unsigned int nest_depth)
{
  const auto indent = std::string((nest_depth + 1) * 2, ' ');
  auto check_result = m_gross_check.checkParameter(data, parameter, nest_depth);

  if (not check_result.m_error.empty()) return check_result;

  const auto& data_value = data.value();
  const auto parameter_scalar_type = parameter.scalarType();

  if (not data_value.isConvertibleToType(parameter_scalar_type))
  {
    check_result.m_error +=
      indent + "Item \"" + data.name() + "\" is required to be of type " +
      varyingDataTypeStringName(parameter_scalar_type) + ". Supplied type is " +
      data_value.typeName() + ", which is not compatible.\n";
  }

  return check_result;
}

// ###################################################################
ParameterCheckResult
ScalarArrayEntriesTypeMustMatch::checkParameter(const DataTree& data,
                                                const InputParameter& parameter,
                                                const unsigned int nest_depth)
{
  const auto indent = std::string((nest_depth + 1) * 2, ' ');
  ParameterCheckResult check_result;

  //============================================= First check if the gross-type
  //                                              is SEQUENCE
  if (data.grossType() != DataGrossType::SEQUENCE)
    check_result.m_error =
      "Item \"" + data.name() +
      "\" is required to be of type SEQUENCE but is of type " +
      DataGrossTypeName(data.grossType()) + ".\n";

  if (not check_result.m_error.empty())
  {
    check_result.m_error.insert(0, indent);
    return check_result;
  }

  //============================================= Now check individual entries
  const auto& entries = data.children();
  size_t entry_index = 0;
  for (const auto& entry : entries)
  {
    const auto entry_name =
      data.name() + "[" + std::to_string(entry_index) + "]";

    if (entry->grossType() != DataGrossType::SCALAR)
    {
      check_result.m_error +=
        "Item \"" + data.name() +
        "\" is required to have entries of gross-type SCALAR but sub-item \"" +
        entry_name + "\" has gross-type of " +
        DataGrossTypeName(entry->grossType()) + ".\n";
      entry_index++;
      continue;
    }
    const auto parameter_scalar_type = parameter.scalarType();
    const auto& entry_value = entry->value();
    if (entry_value.type() != parameter_scalar_type)
    {
      check_result.m_error +=
        "Item \"" + data.name() +
        "\" is required to have entries of scalar-type " +
        varyingDataTypeStringName(parameter_scalar_type) + " but sub-item \"" +
        entry_name + "\" has scalar-type of " + entry_value.typeName() + ".\n";
    }
    entry_index++;
  }

  if (not check_result.m_warning.empty())
    check_result.m_warning.insert(0, indent);
  if (not check_result.m_error.empty()) check_result.m_error.insert(0, indent);

  return check_result;
}

// ###################################################################
ParameterCheckResult ScalarArrayEntriesTypeMustBeCompatible::checkParameter(
  const DataTree& data,
  const InputParameter& parameter,
  const unsigned int nest_depth)
{
  const auto indent = std::string((nest_depth + 1) * 2, ' ');
  ParameterCheckResult check_result;

  //============================================= First check if the gross-type
  //                                              is SEQUENCE
  if (data.grossType() != DataGrossType::SEQUENCE)
    check_result.m_error =
      "Item \"" + data.name() +
      "\" is required to be of type SEQUENCE but is of type " +
      DataGrossTypeName(data.grossType()) + ".\n";

  if (not check_result.m_error.empty())
  {
    check_result.m_error.insert(0, indent);
    return check_result;
  }

  //============================================= Now check individual entries

  const auto& entries = data.children();
  size_t entry_index = 0;
  for (const auto& entry : entries)
  {
    const auto entry_name =
      data.name() + "[" + std::to_string(entry_index) + "]";

    if (entry->grossType() != DataGrossType::SCALAR)
    {
      check_result.m_error +=
        indent + "Item \"" + data.name() +
        "\" is required to have entries of gross-type SCALAR but sub-item \"" +
        entry_name + "\" has gross-type of " +
        DataGrossTypeName(entry->grossType()) + ".\n";
      entry_index++;
      continue;
    }
    const auto parameter_scalar_type = parameter.scalarType();
    const auto& entry_value = entry->value();
    if (not entry_value.isConvertibleToType(parameter_scalar_type))
    {
      check_result.m_error +=
        indent + "Item \"" + data.name() +
        "\" is required to have entries of scalar-type " +
        varyingDataTypeStringName(parameter_scalar_type) + " but sub-item \"" +
        entry_name + "\" has scalar-type of " + entry_value.typeName() +
        ", which is not compatible.\n";
    }
    entry_index++;
  }

  return check_result;
}

// ###################################################################
ParameterCheckResult RegisteredObjectMustExist::checkParameter(
  const DataTree& data, const InputParameter&, const unsigned int nest_depth)
{
  const auto indent = std::string((nest_depth + 1) * 2, ' ');
  ParameterCheckResult check_result;
  const auto context = indent + data.name() + ": ";

  //============================================= Check the data tree is a map
  if (data.grossType() != DataGrossType::MAP)
  {
    check_result.m_error =
      context + "The tree must be of gross-type MAP but is of type " +
      DataGrossTypeName(data.grossType()) + ". At minimum it must have the " +
      "\"type\" entry.\n";
    return check_result;
  }

  //============================================= Check it has the "type" param
  if (not data.hasChild("type"))
  {
    check_result.m_error =
      context + "The tree does not have a \"type\" entry. Therefore no "
                "registered object can be deduced.\n";
    return check_result;
  }

  //============================================= Check the "type" param is a
  //                                              string
  const auto& type_entry = data.child("type");
  if (type_entry.value().type() != VaryingDataType::STRING)
  {
    check_result.m_error =
      context +
      "The tree's \"type\" entry must be of type STRING but is of type " +
      type_entry.value().typeName() + ".\n";
    return check_result;
  }

  //============================================= Check the "type" string points
  //                                              to a registered object.
  const auto type_entry_string = type_entry.value().stringValue();
  const auto& factory_object_register =
    elke::StaticRegister::getFactoryObjectRegister();
  const auto find_result = factory_object_register.find(type_entry_string);
  if (find_result == factory_object_register.end())
  {
    check_result.m_error =
      context +
      R"(A registered object was not found for "type" entry with value ")" +
      type_entry_string + "\".\n";
    return check_result;
  }

  //============================================= Run the factory object's
  //                                              Checks
  const auto& registry_entry = find_result->second;
  const auto object_input_parameters = registry_entry.m_parameter_function();

  WarningsAndErrorsData warnings_and_errors_data;
  object_input_parameters.checkInputDataValidity(
    data, warnings_and_errors_data, nest_depth + 1);

  for (const auto& message : warnings_and_errors_data.m_warnings)
    check_result.m_warning += message;
  for (const auto& message : warnings_and_errors_data.m_errors)
    check_result.m_error += message;

  const auto data_label =
    data.name().empty() ? "" : indent + data.name() + ": ";

  if (not check_result.m_warning.empty())
    check_result.m_warning.insert(0, data_label);
  if (not check_result.m_error.empty())
    check_result.m_error.insert(0, data_label);

  return check_result;
}

// ###################################################################
ParameterCheckResult RegisteredObjectArrayEntriesMustExist::checkParameter(
  const DataTree& data,
  const InputParameter& parameter,
  const unsigned int nest_depth)
{
  const auto indent = std::string((nest_depth + 1) * 2, ' ');
  RegisteredObjectMustExist single_check;
  ParameterCheckResult check_result;

  //============================================= Check the data tree is a map
  if (data.grossType() != parameter.grossType())
  {
    check_result.m_error =
      indent + data.name() + ": " + "The data-input must be of gross-type " +
      DataGrossTypeName(parameter.grossType()) + " but is of type " +
      DataGrossTypeName(data.grossType()) +
      ". I.e., it is expected to be an array of MAP types.\n";
    return check_result;
  }

  //============================================= Check each entry
  size_t entry_index = 0;
  for (const auto& child_ptr : data.children())
  {
    const std::string entry_name =
      indent + data.name() + "[" + std::to_string(entry_index) + "]";

    auto entry_check_result =
      single_check.checkParameter(*child_ptr, parameter /*dummy*/, nest_depth);

    if (not entry_check_result.m_warning.empty())
      check_result.m_warning +=
        entry_name + ": " + entry_check_result.m_warning;

    if (not entry_check_result.m_error.empty())
      check_result.m_error += entry_name + ": " + entry_check_result.m_error;

    entry_index++;
  }

  return check_result;
}

// ###################################################################
ParameterCheckResult RegisteredObjectMapEntriesMustExist::checkParameter(
  const DataTree& data,
  const InputParameter& parameter,
  const unsigned int nest_depth)
{
  const auto indent = std::string((nest_depth + 1) * 2, ' ');
  RegisteredObjectMustExist single_check;

  ParameterCheckResult check_result;

  //============================================= Check the data tree is a map
  if (data.grossType() != parameter.grossType())
  {
    check_result.m_error =
      indent + data.name() + ": " + "The data-input must be of gross-type " +
      DataGrossTypeName(parameter.grossType()) + " but is of type " +
      DataGrossTypeName(data.grossType()) +
      ". I.e., it is expected to be a MAP of MAP types.\n";
    return check_result;
  }

  //============================================= Check each entry
  size_t entry_index = 0;
  for (const auto& child_ptr : data.children())
  {
    const std::string entry_name = indent + data.name();

    auto entry_check_result =
      single_check.checkParameter(*child_ptr, parameter /*dummy*/, nest_depth);

    if (not entry_check_result.m_warning.empty())
      check_result.m_warning +=
        entry_name + ": " + entry_check_result.m_warning;

    if (not entry_check_result.m_error.empty())
      check_result.m_error += entry_name + ": " + entry_check_result.m_error;

    entry_index++;
  }

  return check_result;
}

// ###################################################################
ParameterCheckResult
NamedInputBlockCheck::checkParameter(const DataTree& data,
                                     const InputParameter& parameter,
                                     unsigned int nest_depth)
{
  const auto indent = std::string((nest_depth + 1) * 2, ' ');

  ParameterCheckResult check_result;
  const auto context = indent + data.name() + ": ";

  //============================================= Check the data tree is a map
  if (data.grossType() != parameter.grossType())
  {
    check_result.m_error =
      indent + data.name() + ": " + "The data-input must be of gross-type " +
      DataGrossTypeName(parameter.grossType()) + " but is of type " +
      DataGrossTypeName(data.grossType()) +
      ". I.e., it is expected to be a MAP of MAP types.\n";
    return check_result;
  }

  //============================================= Downcast the parameter
  const auto block_param_ptr =
    dynamic_cast<NamedBlockInputParameter const*>(&parameter);

  if (not block_param_ptr)
  {
    check_result.m_error +=
      "Failed obtaining a reference to NamedBlockInputParameter\n";
    return check_result;
  }

  //============================================= Check the block_name string
  //                                              points to a registered input
  //                                              block.
  const auto block_name = block_param_ptr->blockName();
  const auto& input_block_register =
    elke::StaticRegister::getInputParameterBlockRegistry();
  const auto find_result = input_block_register.find(block_name);
  if (find_result == input_block_register.end())
  {
    check_result.m_error = context +
                           R"(A named input-block was not found for name ")" +
                           block_name + "\".\n";
    return check_result;
  }

  //============================================= Run the block's own tests
  const auto& registry_entry = find_result->second;
  const auto object_input_parameters = registry_entry.m_parameter_function();

  WarningsAndErrorsData warnings_and_errors_data;
  object_input_parameters.checkInputDataValidity(
    data, warnings_and_errors_data, nest_depth + 1);

  for (const auto& message : warnings_and_errors_data.m_warnings)
    check_result.m_warning += message;
  for (const auto& message : warnings_and_errors_data.m_errors)
    check_result.m_error += message;

  const auto data_label =
    data.name().empty() ? "" : indent + data.name() + ": ";

  if (not check_result.m_warning.empty())
    check_result.m_warning.insert(0, data_label);
  if (not check_result.m_error.empty())
    check_result.m_error.insert(0, data_label);

  return check_result;
}

// ###################################################################
ParameterCheckResult
ArrayOfNamedInputBlocksCheck::checkParameter(const DataTree& data,
                                             const InputParameter& parameter,
                                             unsigned int nest_depth)
{
  const auto indent = std::string((nest_depth + 1) * 2, ' ');

  ParameterCheckResult check_result;
  const auto context = indent + data.name() + ": ";

  //============================================= Check the data tree type
  if (data.grossType() != parameter.grossType())
  {
    check_result.m_error =
      indent + data.name() + ": " + "The data-input must be of gross-type " +
      DataGrossTypeName(parameter.grossType()) + " but is of type " +
      DataGrossTypeName(data.grossType()) +
      ". I.e., it is expected to be a MAP of MAP types.\n";
    return check_result;
  }

  //============================================= Downcast the parameter
  const auto block_param_ptr =
    dynamic_cast<VectorOfNamedBlocksInputParameter const*>(&parameter);

  if (not block_param_ptr)
  {
    check_result.m_error +=
      "Failed obtaining a reference to VectorOfNamedBlocksInputParameter\n";
    return check_result;
  }

  //============================================= Check the block_name string
  //                                              points to a registered input
  //                                              block.
  const auto block_name = block_param_ptr->blockName();
  const auto& input_block_register =
    elke::StaticRegister::getInputParameterBlockRegistry();
  const auto find_result = input_block_register.find(block_name);
  if (find_result == input_block_register.end())
  {
    check_result.m_error = context +
                           R"(A named input-block was not found for name ")" +
                           block_name + "\".\n";
    return check_result;
  }

  //============================================= Run the block's own tests
  const auto& registry_entry = find_result->second;
  const auto object_input_parameters = registry_entry.m_parameter_function();

  size_t id = 0;
  for (const auto& child_ptr : data.children())
  {
    const auto& child_data = *child_ptr;

    ParameterCheckResult child_check_result;

    WarningsAndErrorsData warnings_and_errors_data;
    object_input_parameters.checkInputDataValidity(
      child_data, warnings_and_errors_data, nest_depth + 1);

    for (const auto& message : warnings_and_errors_data.m_warnings)
      child_check_result.m_warning += message;
    for (const auto& message : warnings_and_errors_data.m_errors)
      child_check_result.m_error += message;

    const auto child_data_label =
      data.name().empty()
        ? ""
        : indent + data.name() + "[" + std::to_string(id) + "]: ";

    if (not child_check_result.m_warning.empty())
      check_result.m_warning += child_data_label + child_check_result.m_warning;
    if (not child_check_result.m_error.empty())
      check_result.m_error += child_data_label + child_check_result.m_error;

    id++;
  } // for child

  return check_result;
}

// ###################################################################
ParameterCheckResult FixedArrayOfNamedInputBlocksCheck::checkParameter(
  const DataTree& data,
  const InputParameter& parameter,
  unsigned int nest_depth)
{
  const auto indent = std::string((nest_depth + 1) * 2, ' ');

  ParameterCheckResult check_result;
  const auto context = indent + data.name() + ": ";

  //============================================= Check the data tree type
  if (data.grossType() != parameter.grossType())
  {
    check_result.m_error =
      indent + data.name() + ": " + "The data-input must be of gross-type " +
      DataGrossTypeName(parameter.grossType()) + " but is of type " +
      DataGrossTypeName(data.grossType()) +
      ". I.e., it is expected to be a MAP of MAP types.\n";
    return check_result;
  }

  //============================================= Downcast the parameter
  const auto block_param_ptr =
    dynamic_cast<FixedVectorOfNamedBlocksInputParameter const*>(&parameter);

  if (not block_param_ptr)
  {
    check_result.m_error +=
      "Failed obtaining a reference to FixedVectorOfNamedBlocksInputParameter\n";
    return check_result;
  }

  const auto& block_names = block_param_ptr->blockNames();

  //============================================= Check the data size matches
  const auto num_children = data.numChildren();
  const auto num_block_names = block_names.size();
  if (num_children != num_block_names)
  {
    check_result.m_error += context + "Data tree has " +
                            std::to_string(num_children) +
                            " entries but requires entries for block names [";
    for (const auto& block_name : block_names)
    {
      check_result.m_error += "\"" + block_name + "\"";
      if (block_name != block_names.back()) check_result.m_error += ",";
    }
    check_result.m_error +=
      "] (" + std::to_string(num_block_names) + " block name(s)).\n";
  }

  //============================================= Process each data entry
  const auto& input_block_register =
    elke::StaticRegister::getInputParameterBlockRegistry();

  size_t id = 0;
  for (const auto& child_ptr : data.children())
  {
    const auto& child_data = *child_ptr;
    const auto& block_name = block_names.at(id);

    //================================= Check the block_name string points to a
    //                                  registered input block.

    const auto find_result = input_block_register.find(block_name);
    if (find_result == input_block_register.end())
    {
      check_result.m_error = context +
                             R"(A named input-block was not found for name ")" +
                             block_name + "\".\n";
      return check_result;
    }

    //================================= Run the blocks own tests
    const auto& registry_entry = find_result->second;
    const auto object_input_parameters = registry_entry.m_parameter_function();

    ParameterCheckResult child_check_result;

    WarningsAndErrorsData warnings_and_errors_data;
    object_input_parameters.checkInputDataValidity(
      child_data, warnings_and_errors_data, nest_depth + 1);

    for (const auto& message : warnings_and_errors_data.m_warnings)
      child_check_result.m_warning += message;
    for (const auto& message : warnings_and_errors_data.m_errors)
      child_check_result.m_error += message;

    const auto child_data_label =
      data.name().empty()
        ? ""
        : indent + data.name() + "[" + std::to_string(id) + "]: ";

    if (not child_check_result.m_warning.empty())
      check_result.m_warning += child_data_label + child_check_result.m_warning;
    if (not child_check_result.m_error.empty())
      check_result.m_error += child_data_label + child_check_result.m_error;

    id++;

    if (id >= num_block_names) break;
  }

  return check_result;
}

// ###################################################################
ParameterCheckResult
MapOfNamedInputBlocksCheck::checkParameter(const DataTree& data,
                                           const InputParameter& parameter,
                                           unsigned int nest_depth)
{
  const auto indent = std::string((nest_depth + 1) * 2, ' ');

  ParameterCheckResult check_result;
  const auto context = indent + data.name() + ": ";

  //============================================= Check the data tree type
  if (data.grossType() != parameter.grossType())
  {
    check_result.m_error =
      indent + data.name() + ": " + "The data-input must be of gross-type " +
      DataGrossTypeName(parameter.grossType()) + " but is of type " +
      DataGrossTypeName(data.grossType()) +
      ". I.e., it is expected to be a MAP of MAP types.\n";
    return check_result;
  }

  //============================================= Downcast the parameter
  const auto block_param_ptr =
    dynamic_cast<MapOfNamedBlocksInputParameter const*>(&parameter);

  if (not block_param_ptr)
  {
    check_result.m_error +=
      "Failed obtaining a reference to MapOfNamedBlocksInputParameter\n";
    return check_result;
  }

  //============================================= Check the block_name string
  //                                              points to a registered input
  //                                              block.
  const auto block_name = block_param_ptr->blockName();
  const auto& input_block_register =
    elke::StaticRegister::getInputParameterBlockRegistry();
  const auto find_result = input_block_register.find(block_name);
  if (find_result == input_block_register.end())
  {
    check_result.m_error = context +
                           R"(A named input-block was not found for name ")" +
                           block_name + "\".\n";
    return check_result;
  }

  //============================================= Run the block's own tests
  const auto& registry_entry = find_result->second;
  const auto object_input_parameters = registry_entry.m_parameter_function();

  size_t id = 0;
  for (const auto& child_ptr : data.children())
  {
    const auto& child_data = *child_ptr;

    ParameterCheckResult child_check_result;

    WarningsAndErrorsData warnings_and_errors_data;
    object_input_parameters.checkInputDataValidity(
      child_data, warnings_and_errors_data, nest_depth + 1);

    for (const auto& message : warnings_and_errors_data.m_warnings)
      child_check_result.m_warning += message;
    for (const auto& message : warnings_and_errors_data.m_errors)
      child_check_result.m_error += message;

    const auto child_data_label =
      data.name().empty()
        ? ""
        : indent + data.name() + "[\"" + child_data.name() + "\"]: ";

    if (not child_check_result.m_warning.empty())
      check_result.m_warning += child_data_label + child_check_result.m_warning;
    if (not child_check_result.m_error.empty())
      check_result.m_error += child_data_label + child_check_result.m_error;

    id++;
  } // for child

  return check_result;
}

} // namespace elke::param_checks