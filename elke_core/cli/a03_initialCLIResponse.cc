#include <iostream>

#include "elke_core/globals/Globals.h"
#include "elke_core/cli/CommandLineArgument.h"
#include "elke_core/elke_configuration.h"

#include <sstream>

namespace elk
{

// ###################################################################
/**Prints header information.*/
void printHeader()
{
  const auto& globals = elk::Globals::getInstance();
  const auto& supplied_clas = globals.getSuppliedCLAs();

  std::stringstream outstr;

  outstr << globals.getProgramHeader() << "\n";
  outstr << "Version: " << PROJECT_VERSION << "\n\n";
  outstr << "Valid command line arguments supplied:\n";

  for (const auto& cla : supplied_clas.getCLAlist())
  {
    const auto column1 = "--" + cla.m_name + ", -" + cla.m_short_name;
    const int offset = 20 - column1.size();

    outstr << column1 + std::string(offset, ' ');
    for (const auto& val : cla.m_values_assigned)
    {
      outstr << val.StringValue();
      if (&val != &*cla.m_values_assigned.rbegin()) outstr << ",";
    }
    outstr << "\n";
  }
  outstr << "\n";

  std::cout << outstr.str(); // TODO: Change to logging
}

// ###################################################################
/**Print help.*/
void printHelp()
{
  const auto& globals = elk::Globals::getInstance();
  const auto& supplied_clas = globals.getSuppliedCLAs();

  std::stringstream outstr;

  outstr << "Command line arguments specification:\n";
  for (const auto& cla : globals.getRegisteredCLAs().getCLAlist())
  {
    const auto column1 = "--" + cla.m_name + ", -" + cla.m_short_name;
    const int offset = 20 - column1.size();

    outstr << column1;
    if (offset < 0) outstr << "\n" + std::string(20, ' ');
    else outstr << std::string(offset, ' ');

    outstr << cla.m_doc_string;

    outstr << "\n";
  }
  outstr << "\n";

  std::cout << outstr.str(); // TODO: Change to logging
}

// ###################################################################
//**This routine contains code to respond to certain global command line
// arguments*/
void initialCLIResponse()
{
  const auto& globals = elk::Globals::getInstance();
  const auto& supplied_clas = globals.getSuppliedCLAs();

  printHeader();
  if (supplied_clas.has("help")) printHelp();
}

} // namespace elk
