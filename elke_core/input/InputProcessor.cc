#include "InputProcessor.h"

#include "YAMLInput.h"
#include "elke_core/output/elk_exceptions.h"
#include "elke_core/output/Logger.h"
#include "elke_core/registration/registration.h"
#include "elke_core/parameters/InputParametersBlock.h"
#include "elke_core/utilities/string_utils.h"

#include <sstream>
#include <utility>
#include <fstream>

namespace elke
{

/**Protected constructor.*/
InputProcessor::InputProcessor(std::shared_ptr<elke::Logger> logger_ptr)
  : m_logger_ptr(std::move(logger_ptr)), m_main_data_tree("")
{
}

// ###################################################################
/**Add a path from which to process an input file.*/
void InputProcessor::addInputFilePath(const std::filesystem::path& path)
{
  m_input_file_paths.emplace_back(path);
}

// ###################################################################
/**Parses input files into data trees using a file-appropriate parser.*/
void InputProcessor::parseInputFiles()
{
  if (m_input_file_paths.empty()) return;

  std::vector<std::string> parsing_warnings;
  std::vector<std::string> parsing_errors;

  //============================================= Identify parser
  for (const auto& path : m_input_file_paths)
  {
    // Check that the file exists
    if (not std::filesystem::is_regular_file(path))
    {
      parsing_errors.push_back("There is no file at \"" + path.string() +
                               "\"\n");
      continue;
    }

    // ReSharper disable once CppDFAConstantConditions
    if (m_echo_input)
    {
      // ReSharper disable once CppDFAUnreachableCode
      std::ifstream file(path, std::ios::in);
      if (file.is_open())
      {
        using iterator = std::istreambuf_iterator<char>;
        const std::string content((iterator(file)), iterator());

        m_logger_ptr->log() << "Input file echo for " << path.string() << ":\n"
                            << content;
      }
      file.close();
    }

    const auto extension = path.extension();
    std::unique_ptr<elke::InputParser> parser_ptr = nullptr;
    if (extension == ".yaml")
    {
      parser_ptr = std::make_unique<YAMLInput>(*m_logger_ptr);
    }
    else
    {
      parsing_errors.emplace_back("No available input parser for extension " +
                                  extension.string() + " of input file " +
                                  path.string() + "\n");
    }

    if (parser_ptr != nullptr)
    {
      elke::DataTree data_tree = parser_ptr->parseInputFile(path);
      const auto& warnings = parser_ptr->warnings();
      const auto& errors = parser_ptr->errors();

      if (not warnings.empty())
        for (const auto& warning : warnings)
          parsing_warnings.push_back(warning);

      if (not errors.empty())
        for (const auto& error : errors)
          parsing_errors.push_back(error);

      //
      // ReSharper disable once CppDFAConstantConditions
      if (m_echo_input_data)
      {
        // ReSharper disable once CppDFAUnreachableCode
        m_logger_ptr->log() << "Input data echo for " << path.string() << ":\n"
                            << data_tree.toStringAsYAML("", {"type"});
      }

      m_data_trees.insert(std::make_pair(path, data_tree));
    }
  } // for path

  //============================================= Print errors & warnings
  if (not parsing_warnings.empty())
  {
    std::stringstream out_stream;
    for (const auto& warning : parsing_warnings)
      out_stream << warning;
    m_logger_ptr->warn() << out_stream.str();
  }

  if (not parsing_errors.empty())
  {
    std::stringstream out_stream;
    out_stream << "\n";
    for (const auto& error : parsing_errors)
    {
      out_stream << error;
      if (not error.empty() and error.back() != '\n') out_stream << '\n';
    }
    elkLogicalError(out_stream.str() +
                    "\nError(s) during input processing (parsing).");
  }

  this->consolidateBlocks();
}

// ###################################################################
void InputProcessor::consolidateBlocks()
{
  // TODO: Implement properly

  if (not m_data_trees.empty()) m_main_data_tree = m_data_trees.begin()->second;
}

// ###################################################################
/**Cascades down from syntax blocks, first checking the input syntax for
 *blocks themselves, then any child blocks.*/
void InputProcessor::checkInputDataForSyntaxBlocks() const
{
  WarningsAndErrorsData warnings_and_errors_data;

  const auto& syntax_block_reg_entries =
    StaticRegister::getSyntaxSystemRegister();

  const auto& main_input_tree_blocks = m_main_data_tree.children();

  for (const auto& [block_name, block_reg_entry] : syntax_block_reg_entries)
  {
    m_logger_ptr->log() << "Processing " << block_reg_entry.m_syntax
                        << " block";
    for (const auto& block_tree_ptr : main_input_tree_blocks)
      if (block_tree_ptr->name() == block_reg_entry.m_syntax)
      {
        m_logger_ptr->log() << "Tree has " << block_tree_ptr->name() << "\n";
        checkInputParameters(block_name,
                             block_reg_entry.m_parameter_function(),
                             *block_tree_ptr,
                             warnings_and_errors_data/*in/out*/);
        break;
      }
  }

  const auto& input_check_errors = warnings_and_errors_data.m_errors;
  if (not input_check_errors.empty())
  {
    std::stringstream out_stream;
    out_stream << "\n";
    for (const auto& error : input_check_errors)
    {
      out_stream << "-----\n" + error + "-----\n";
      if (not error.empty() and error.back() != '\n') out_stream << '\n';
    }
    elkLogicalError(out_stream.str() +
                    "\nError(s) during input processing (checking).");
  }
}

// ###################################################################
void InputProcessor::checkInputParameters(
  const std::string& item_name,
  const InputParametersBlock& in_params,
  const DataTree& data,
  WarningsAndErrorsData& warnings_and_errors_data)
{
  const std::string context = "While checking input parameters for "
                              "block \"" +
                              in_params.name() + "\" from data-tree \"" +
                              data.getTag("address") + "\":\n";

  auto& input_check_errors = warnings_and_errors_data.m_errors;

  //=================================== First we check if the data is actually
  //                                    valid, if not we mark them to be skipped
  //                                    because we provide an error for them
  //                                    once
  std::vector<std::string> invalid_param_names;

  for (const auto& child : data.children())
    if (not in_params.hasParameter(child->name()))
      invalid_param_names.emplace_back(child->name());

  //=================================== For each of the invalid parameters
  //                                    check if we have a suggestion.
  if (not invalid_param_names.empty())
  {
    std::unordered_set<std::string> valid_param_names_set;
    for (const auto& child : in_params)
      valid_param_names_set.insert(child.name());

    for (const auto& invalid_param_name : invalid_param_names)
    {
      const auto suggestion = string_utils::findClosestMatchingString(
        invalid_param_name, valid_param_names_set);

      const std::string suggestion_str =
        suggestion.empty()
          ? " No suggested parameter name could be determined.\n"
          : " Did you mean \"" + suggestion + "\"?\n";

      input_check_errors.emplace_back(
        context +
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

    const auto& input_parameter = in_params.getParameter(child.name());

    auto result = input_parameter.performChecks(child);

    if (not result.m_error.empty())
      input_check_errors.emplace_back(context + result.m_error);
  } // for child

  //=================================== Now we check if required parameters
  //                                    have been supplied
  for (const auto& parameter : in_params)
    if (parameter.classType() == ParameterClass::REQUIRED and
        not data.hasChild(parameter.name()))
      input_check_errors.emplace_back(context + "Required parameter \"" +
                                      parameter.name() + "\" not supplied.\n");
}

} // namespace elke
