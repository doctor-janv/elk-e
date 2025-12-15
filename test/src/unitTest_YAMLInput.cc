#include "elke_core/input/YAMLInput.h"
#include "elke_core/FrameworkCore.h"

namespace elke::unit_tests
{

void unitTestYAMLInput()
{
  auto& logger = FrameworkCore::getInstance().getLogger();

  YAMLInput input_processor(logger);

  const auto data_tree = input_processor.parseInputFile("YAMLInput.yaml");
  logger.log() << data_tree.toStringAsYAML("", {"type", "address", "mark"});
}

} // namespace elke::unit_tests

elkeRegisterNullaryFunction(elke::unit_tests::unitTestYAMLInput);