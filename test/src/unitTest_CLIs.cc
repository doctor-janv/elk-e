#include "elke_core/FrameworkCore.h"

namespace elke::unit_tests
{

/**Routine to test basic functionality of CLIs */
void unitTest_CLIs()
{
  const auto& core_module = FrameworkCore::getInstance();

  const auto& supplied_CLAs = core_module.getSuppliedCommandLineArguments();

  for (const auto& cla : supplied_CLAs.getCLAlist())
  {
    std::cout << cla.m_name << std::endl;
    for (const auto& value : cla.m_values_assigned)
      std::cout << value.PrintStr() << std::endl;
  }
}

} // namespace elke::unit_tests

elkeRegisterNullaryFunction(elke::unit_tests::unitTest_CLIs);
