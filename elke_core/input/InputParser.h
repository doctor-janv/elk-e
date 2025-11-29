#ifndef ELK_E_INPUTPROCESSOR_H
#define ELK_E_INPUTPROCESSOR_H
#include "elke_core/data_types/DataTree.h"

#include <vector>
#include <string>

namespace elke
{
class Logger;

/**Base class for input processors.*/
class InputParser
{
protected:
  explicit InputParser(elke::Logger& logger);
  elke::Logger& m_logger;
  std::vector<std::string> m_warnings;
  std::vector<std::string> m_errors;

public:
  /**Reads an input file and produces a DataTree.*/
  virtual elke::DataTree parseInputFile(std::string file_name) = 0;
  virtual ~InputParser() = default;

  const std::vector<std::string>& warnings() const;
  const std::vector<std::string>& errors() const;

};

} // namespace elke

#endif // ELK_E_INPUTPROCESSOR_H
