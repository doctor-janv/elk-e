#include "elke.h"

#include <iostream>

namespace elk
{

int main(int argc, char** argv)
{
  try
  {
    elk::setupGlobalCLI();
    elk::parseCommandLine(argc, argv);
    elk::initialCLIResponse();
  }
  catch(const std::exception& exception_object)
  {
    std::cout << exception_object.what() << std::endl;
  }

  return 0;
}

}
