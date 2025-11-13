#include "CommandLineInterface.h"

#include "CommandLineArgument.h"
#include "elke_core/output/elk_exceptions.h"

#include "elke_core/elke_configuration.h"

#include <sstream>
#include <utility>

namespace elke
{

// ###################################################################
CommandLineInterface::CommandLineInterface(std::shared_ptr<Logger> logger_ptr)
  : m_program_header(
      CommandLineInterface::constructionHelperSetDefaultHeader()),
    m_logger_ptr(std::move(logger_ptr))
{
}

// ###################################################################
void CommandLineInterface::registerNewCLA(const CommandLineArgument& cla)
{
  elkInvalidArgumentIf(m_registered_command_line_arguments.has(cla.m_name),
                       "CLA with name \"" + cla.m_name +
                         "\" already registered");

  m_registered_command_line_arguments.add(cla);
}

// ###################################################################
const CommandLineArgumentList&
CommandLineInterface::getRegisteredCommandLineArguments() const
{
  return m_registered_command_line_arguments;
}

// ###################################################################
const CommandLineArgumentList&
CommandLineInterface::getSuppliedCommandLineArguments() const
{
  return m_supplied_command_line_arguments;
}

// ###################################################################
void CommandLineInterface::parseCommandLine(const int argc, char** argv)
{
  //=================================== Converts c-strings to std::string
  std::vector<std::string> raw_clas;
  raw_clas.reserve(argc);
  for (int c = 0; c < argc; ++c)
    raw_clas.emplace_back(argv[c]);

  this->parseCommandLine(raw_clas);
}

// ###################################################################
void CommandLineInterface::parseCommandLine(
  const std::vector<std::string>& args)
{
  //=================================== Parse out CLAs
  const auto& cla_specs = m_registered_command_line_arguments;
  const size_t argc = args.size();
  for (size_t c = 1; c < argc; ++c) // Skipping c[0] (normally executable)
  {
    const auto& cla = args.at(c);

    //============================ Figure out short or long name
    std::string short_name;
    std::string long_name;

    if (cla.find('-') != std::string::npos and cla.size() == 2)
      short_name = cla.substr(1, 1);
    else if (cla.find("--") != std::string::npos)
      long_name = cla.substr(2, cla.size() - 2);

    //============================ Check this is a valid CLA
    if (not(cla_specs.hasShortName(short_name) or cla_specs.has(long_name)))
      elkLogicalError("Invalid argument \"" + cla + "\"");

    //============================ Grab the CLA specification
    const auto& cla_spec = short_name.empty()
                             ? cla_specs.getCLAbyName(long_name)
                             : cla_specs.getCLAbyShortName(short_name);

    //============================ Add the CLA to the "supplied" list
    // If it is already there, check
    // if that is allowed
    auto& supplied_clas = m_supplied_command_line_arguments;
    if (not supplied_clas.has(cla_spec.m_name)) supplied_clas.add(cla_spec);
    else if (cla_spec.m_only_one_allowed)
      elkLogicalError("Command line argument --" + cla_spec.m_name + "/-" +
                      cla_spec.m_short_name + " is only allowed once.");

    //============================ Check if actual value for key is required
    if (cla_spec.m_requires_value)
    {
      if (c + 1 >= argc)
        elkLogicalError("Command line argument --" + cla_spec.m_name + "/-" +
                        cla_spec.m_short_name +
                        " requires a value to be supplied but the keyword " +
                        "specifier is at the end of command line," +
                        " so a value cannot be deduced for it.");

      const auto& next_cla = args.at(c + 1);
      if ((next_cla.find('-') == 0 and next_cla.size() == 2) or
          next_cla.find("--") == 0)
        elkLogicalError("Command line argument --" + cla_spec.m_name + "/-" +
                        cla_spec.m_short_name +
                        " requires a value to be supplied but the keyword " +
                        "is followed by another keyword specifier, " +
                        next_cla + ", therefore no value can be deduced.");

      const auto value = Varying(next_cla);
      c += 1;
      auto& supplied_cla = supplied_clas.getCLAbyName(cla_spec.m_name);
      supplied_cla.m_values_assigned.push_back(value);
    }

  } // for int c
}

// ###################################################################
void CommandLineInterface::registerCommonCLI_Items()
{
  auto cli0 = CommandLineArgument("help",
                                  "h",
                                  "Prints basic help for the program.",
                                  /*default_value=*/Varying(0),
                                  /*only_one_allowed=*/false,
                                  /*requires_value=*/false);
  auto cli1 = CommandLineArgument("input", "i", "Input file to the program.");
  auto cli2 = CommandLineArgument(
    "verbosity", "v", "Verbosity level. 0=default, 1=more, 2=most.");

  auto cli3 = CommandLineArgument("nocolor",
                                  "",
                                  "Suppresses color output.",
                                  /*default_value=*/Varying(false),
                                  /*only_one_allowed=*/true,
                                  /*requires_value=*/false);

  this->registerNewCLA(cli0);
  this->registerNewCLA(cli1);
  this->registerNewCLA(cli2);
  this->registerNewCLA(cli3);
}

// ###################################################################
void CommandLineInterface::respondToCommonCLAs() const
{
  const auto& supplied_clas = getSuppliedCommandLineArguments();
  auto& logger = *m_logger_ptr;

  this->printHeader();
  if (supplied_clas.has("help")) this->printHelp();

  if (supplied_clas.has("nocolor")) logger.setColorSuppression(true);

  if (supplied_clas.has("verbosity"))
  {
    const auto& cla = supplied_clas.getCLAbyName("verbosity");
    logger.setVerbosity(
      static_cast<int>(cla.m_values_assigned.front().IntegerValue()));
  }
}

// ###################################################################
const std::string& CommandLineInterface::getHeader() const
{
  return m_program_header;
}

// ###################################################################
void CommandLineInterface::setHeader(std::string new_header)
{
  m_program_header = std::move(new_header);
}


// ###################################################################
std::string CommandLineInterface::constructionHelperSetDefaultHeader()
{
  // clang-format off
  std::string header =
"*******************************************************************************\n"
"*                                 ELKE                                        *\n"
"*                    A computational physics library                          *\n"
"*                          for thermal-hydraulics                             *\n"
"*******************************************************************************\n";
  header += "Framework Version: " + std::string(PROJECT_VERSION) + "\n";
  // clang-format on
  return header;
}

// ###################################################################
void CommandLineInterface::printHelp() const
{
  const auto& registered_clas = this->getRegisteredCommandLineArguments();
  auto& logger = *m_logger_ptr;

  std::stringstream outstr;

  outstr << "Command line arguments specification:\n";
  for (const auto& cla : registered_clas.getCLAlist())
  {
    const std::string short_part =
      cla.m_short_name.empty() ? "" : ", -" + cla.m_short_name;
    const auto column1 = "--" + cla.m_name + short_part;
    const int offset = std::max(20 - static_cast<int>(column1.size()), 0);

    outstr << column1;
    if (offset < 0) outstr << "\n" + std::string(20, ' ');
    else
      outstr << std::string(offset, ' ');

    outstr << cla.m_doc_string;

    outstr << "\n";
  }
  outstr << "\n";

  logger.log() << outstr.str();
}

// ###################################################################
void CommandLineInterface::printHeader() const
{
  const auto& supplied_clas = this->getSuppliedCommandLineArguments();
  auto& logger = *m_logger_ptr;

  std::stringstream outstr;

  outstr << m_program_header << "\n\n";

  const auto& supplied_clas_list = supplied_clas.getCLAlist();
  if (not supplied_clas_list.empty())
    outstr << "Valid command line arguments supplied:\n";
  else
    outstr << logger.stringColor(FG_YELLOW) +
                "No command line arguments have been supplied.\n" +
                logger.stringColor(RESET);

  for (const auto& cla : supplied_clas_list)
  {
    const std::string short_part =
      cla.m_short_name.empty() ? "" : ", -" + cla.m_short_name;
    const auto column1 = "--" + cla.m_name + short_part;
    const int offset = std::max(20 - static_cast<int>(column1.size()), 0);

    outstr << column1 + std::string(offset, ' ');
    for (const auto& val : cla.m_values_assigned)
    {
      outstr << val.StringValue();
      if (&val != &cla.m_values_assigned.back()) outstr << ", ";
    }
    outstr << "\n";
  }
  outstr << "\n";

  logger.log() << outstr.str();
}

} // namespace elke