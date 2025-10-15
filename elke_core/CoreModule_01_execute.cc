#include "CoreModule.h"

#include <iostream>

namespace elke
{

//###################################################################
int CoreModule::execute(const int argc, char** argv)
{
  auto& core = CoreModule::getInstance();
  try
  {
    core.registerCLI();
    core.parseCommandLine(argc, argv);
    core.initialCLIResponse();
  }
  catch(const std::exception& exception_object)
  {
    std::cout << exception_object.what() << "\n";
    return 1;
  }

  return 0;
}

extern "C" void elke__CoreModule__execute()
{
  std::cout << "executing core" << "\n";
}



} // namespace elke
