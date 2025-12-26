#include "elke_core/FrameworkCore.h"

namespace elke::unit_tests
{

void unitTest_StackTraces()
{
  elke::FrameworkCore::forcedQuit();
}

} // namespace elke::unit_tests

elkeRegisterNullaryFunction(elke::unit_tests::unitTest_StackTraces);