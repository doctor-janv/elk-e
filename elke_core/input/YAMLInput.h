#ifndef ELK_E_YAMLINPUT_H
#define ELK_E_YAMLINPUT_H

#include "InputProcessor.h"
#include "elke_core/data_types/DataTree.h"
#include "elke_core/output/Logger.h"

namespace elke
{

/**YAML input processor.*/
class YAMLInput final : public InputProcessor
{
private:
  Logger& m_logger;
public:
  YAMLInput(Logger& logger);

  elke::DataTree readInputFile(std::string file_name) override;
};

} // namespace elke

#endif // ELK_E_YAMLINPUT_H
