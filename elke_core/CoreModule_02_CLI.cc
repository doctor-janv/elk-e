#include "CoreModule.h"
#include "elke_core/cli/CommandLineArgument.h"
#include "elke_core/output/elk_exceptions.h"
#include "elke_core/elke_configuration.h"



#include <iostream>
#include <sstream>

namespace elke
{

// ###################################################################
void CoreModule::registerCLI()
{
  ModuleBase::registerCLI();
  auto cli0 = CommandLineArgument("say-hello",
                                  "",
                                  "Prints a greeting.",
                                  /*default_value=*/Varying(int(0)),
                                  /*only_one_allowed=*/true,
                                  /*requires_value=*/false);
  auto cli1 = CommandLineArgument("basic",
                                  "b",
                                  "Calls a basic command in the "
                                  "form 'command arg1 arg2 arg3 etc.'",
                                  /*default_value=*/Varying(""),
                                  /*only_one_allowed=*/false,
                                  /*requires_value=*/true);


  this->registerNewCLA(cli0);
  this->registerNewCLA(cli1);
}

// ###################################################################
void CoreModule::initialCLIResponse()
{
  ModuleBase::initialCLIResponse();


}

} // namespace elke
