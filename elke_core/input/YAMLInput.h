#ifndef ELK_E_YAMLINPUT_H
#define ELK_E_YAMLINPUT_H

#include "InputParser.h"
#include "elke_core/data_types/DataTree.h"

namespace elke
{
class Logger;

/**YAML input processor.*/
class YAMLInput final : public InputParser
{
  /**Flag to print extra input*/
  bool m_test_mode/*=false*/;
public:
  explicit YAMLInput(elke::Logger& logger, bool test_mode = false);

  elke::DataTree parseInputFile(std::string file_name) override;
};

} // namespace elke

#endif // ELK_E_YAMLINPUT_H
