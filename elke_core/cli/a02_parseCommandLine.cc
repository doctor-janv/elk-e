#include "elke_core/cli/CommandLineArgument.h"
#include "elke_core/globals/Globals.h"
#include "elke_core/output/elk_exceptions.h"

namespace elk
{

void parseCommandLine(int argc, char** argv)
{
  auto& globals = elk::Globals::getInstance();

  //=================================== Converts c-strings to std::string
  std::vector<std::string> raw_clas;
  for (int c = 0; c < argc; ++c)
    raw_clas.push_back(argv[c]);

  //=================================== Parse out CLAs
  const auto& cla_specs = globals.getRegisteredCLAs();
  for (int c = 1; c < argc; ++c) // Skipping c[0] (normally executable)
  {
    const auto& cla = raw_clas.at(c);

    //============================ Figure out short or long name
    std::string short_name;
    std::string long_name;

    if (cla.find("-") != std::string::npos and cla.size() == 2)
      short_name = cla.substr(1, 1);
    else if (cla.find("--") != std::string::npos)
      long_name = cla.substr(2, cla.size() - 2);

    //============================ Check this is a valid CLA
    if (not(cla_specs.hasShortName(short_name) or cla_specs.has(long_name)))
      elkLogicalError("Invalid argument \"" + cla + "\"");

    //============================ Grab the CLA specification
    const auto& cla_spec = (short_name == "")
                             ? cla_specs.getCLAbyName(long_name)
                             : cla_specs.getCLAbyShortName(short_name);

    //============================ Add the CLA to the "supplied" list
    // If it is already there, check
    // if that is allowed
    auto& supplied_clas = globals.getSuppliedCLAs();
    if (not supplied_clas.has(cla_spec.m_name)) supplied_clas.add(cla_spec);
    else if (not cla_spec.m_only_one_allowed)
      elkLogicalError("Command line argument --" + cla_spec.m_name + "/-" +
                        cla_spec.m_short_name +
                        " is only allowed once.");

    //============================ Check if actual value for key is required
    if (cla_spec.m_requires_value)
    {
      if ((c + 1) >= argc)
        elkLogicalError("Command line argument --" + cla_spec.m_name + "/-" +
                        cla_spec.m_short_name +
                        " requires a value to be supplied but the keyword " +
                        "specifier is at the end of command line," +
                        " so a value cannot be deduced for it.");

      const auto& next_cla = raw_clas.at(c + 1);
      if ((next_cla.find("-") == 0 and next_cla.size() == 2) or
          next_cla.find("--") == 0)
        elkLogicalError("Command line argument --" + cla_spec.m_name + "/-" +
                        cla_spec.m_short_name +
                        " requires a value to be supplied but the keyword " +
                        "is followed by another keyword specifier, " +
                        next_cla + ", therefore no value can be deduced.");

      const auto value = elk::Varying(next_cla);
      c += 1;
      auto& supplied_cla = supplied_clas.getCLAbyName(cla_spec.m_name);
      supplied_cla.m_values_assigned.push_back(value);
    }

  } // for int c
}

} // namespace elk
