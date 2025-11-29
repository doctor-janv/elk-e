#ifndef ELK_E_INPUTPROCESSINGINTERFACE_H
#define ELK_E_INPUTPROCESSINGINTERFACE_H

#include <vector>
#include <filesystem>
#include <map>

#include "elke_core/data_types/DataTree.h"

namespace elke
{

class Logger;
/**A class for handling input processing.*/
class InputProcessor
{
  const std::shared_ptr<elke::Logger> m_logger_ptr;
  std::vector<std::filesystem::path> m_input_file_paths;
  std::map<std::filesystem::path, elke::DataTree> m_data_trees;
  bool m_echo_input = false;
  bool m_echo_input_data = false;

public:
  /**Protected constructor.*/
  explicit InputProcessor(std::shared_ptr<elke::Logger> logger_ptr);

  /**Add a path from which to process an input file.*/
  void addInputFilePath(const std::filesystem::path& path);

  /**Parses input files into data trees using a file-appropriate parser.*/
  void parseInputFiles();

  /**Turns on/off the echoing of the input files.*/
  void setEchoInput(const bool value) { m_echo_input = value; }
  /**Turns on/off the echoing of the processed input files.*/
  void setEchoInputData(const bool value) { m_echo_input_data = value; }
};

} // namespace elke

#endif // ELK_E_INPUTPROCESSINGINTERFACE_H
