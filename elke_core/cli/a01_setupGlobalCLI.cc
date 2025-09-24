#include "elke_core/globals/Globals.h"
#include "elke_core/cli/CommandLineArgument.h"

namespace elk
{

void setupGlobalCLI()
{
  auto cli0 =
    CommandLineArgument("help", "h", "Prints basic help for the program.",
                        /*default_value=*/0,
                        /*requires_value=*/true);
  auto cli1 = CommandLineArgument("input", "i", "Input file to the program.");
  auto cli2 = CommandLineArgument(
    "verbosity", "v", "Verbosity level. 0=default, 1=more, 2=most.");

  elk::Globals::registerCLI(cli0);
  elk::Globals::registerCLI(cli1);
  elk::Globals::registerCLI(cli2);
}

} // namespace elk
