#include "InputProcessor.h"

#include "YAMLInput.h"
#include "elke_core/output/elk_exceptions.h"
#include "elke_core/output/Logger.h"

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

    if (m_echo_input)
    {
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

      if (m_echo_input_data)
      {
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
    elkLogicalError(out_stream.str() + "\nError(s) during input processing.");
  }
}

// ###################################################################
void InputProcessor::consolidateBlocks()
{
  // TODO: Implement properly

  if (not m_data_trees.empty())
    m_main_data_tree = m_data_trees.begin()->second;
}

} // namespace elke
