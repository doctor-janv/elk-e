#ifndef ELK_E_INPUTPROCESSINGINTERFACE_H
#define ELK_E_INPUTPROCESSINGINTERFACE_H

#include <vector>
#include <filesystem>
#include <map>

#include "elke_core/data_types/DataTree.h"

namespace elke
{

class InputParametersBlock;

class Logger;

struct WarningsAndErrorsData
{
  std::vector<std::string> m_warnings;
  std::vector<std::string> m_errors;
};

/**A class for handling input processing.*/
class InputProcessor
{
  const std::shared_ptr<elke::Logger> m_logger_ptr;

  std::vector<std::filesystem::path> m_input_file_paths;
  std::map<std::filesystem::path, elke::DataTree> m_data_trees;
  elke::DataTree m_main_data_tree;
  bool m_echo_input = false;
  bool m_echo_input_data = false;

public:
  /**Protected constructor.*/
  explicit InputProcessor(
    std::shared_ptr<elke::Logger> logger_ptr);

  /**Add a path from which to process an input file.*/
  void addInputFilePath(const std::filesystem::path& path);

  /**Parses input files into data trees using a file-appropriate parser.*/
  void parseInputFiles();

private:
  /**Consolidate blocks.*/
  void consolidateBlocks();

public:
  /**Cascades down from syntax blocks, first checking the input syntax for
   *blocks themselves, then any child blocks.*/
  void checkInputDataForSyntaxBlocks() const;

  static void
  checkInputParameters(const std::string& item_name,
                       const InputParametersBlock& in_params,
                       const DataTree& data,
                       WarningsAndErrorsData& warnings_and_errors_data);

  // /**Returns the main data tree extracted from input*/
  // const DataTree& mainDataTree() const { return m_main_data_tree; }

  /**Turns on/off the echoing of the input files.*/
  void setEchoInput(const bool value) { m_echo_input = value; }
  /**Turns on/off the echoing of the processed input files.*/
  void setEchoInputData(const bool value) { m_echo_input_data = value; }
};

} // namespace elke

#endif // ELK_E_INPUTPROCESSINGINTERFACE_H
