#ifndef ELK_E_YAMLINPUT_H
#define ELK_E_YAMLINPUT_H

#include "InputParser.h"
#include "elke_core/data_types/DataTree.h"

namespace YAML
{
  class Node;
}
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

private:
#ifdef YAML_CPP_EXISTS
  void populateTree(elke::DataTree& tree,
                  const YAML::Node& node,
                  Logger& logger,
                  const int level,
                  const bool test_mode);
#endif

  std::string m_current_file_name;
};

} // namespace elke

#endif // ELK_E_YAMLINPUT_H
