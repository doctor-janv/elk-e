#include "elke_core/input/YAMLInput.h"
#include "elke_core/FrameworkCore.h"

namespace elke::unit_tests
{

void unitTestYAMLInput()
{
  YAMLInput input_processor(FrameworkCore::getInstance().getLogger());

  input_processor.readInputFile("SampleYAMLInput.yaml");
}

} // namespace elke::unit_tests

elkeRegisterNullaryFunction(elke::unit_tests::unitTestYAMLInput);