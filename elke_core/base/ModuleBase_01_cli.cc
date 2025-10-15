#include "ModuleBase.h"

#include "elke_core/output/elk_exceptions.h"
#include "elke_core/elke_configuration.h"
#include "elke_core/registration/registration.h"
#include "elke_core/utilities/string_utils.h"
#include "elke_core/utilities/general_utils.h"

namespace elke
{

// ###################################################################
void ModuleBase::registerNewCLA(const CommandLineArgument& cla)
{
  elkInvalidArgumentIf(m_registered_command_line_arguments.has(cla.m_name),
                       "CLA with name \"" + cla.m_name +
                         "\" already registered");

  m_registered_command_line_arguments.add(cla);
}

// ###################################################################
void ModuleBase::registerCLI()
{
  auto cli0 = CommandLineArgument("help",
                                  "h",
                                  "Prints basic help for the program.",
                                  /*default_value=*/Varying(int(0)),
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
  auto cli4 = CommandLineArgument("dump-registry",
                                  "",
                                  "Dumps the registry in yaml format",
                                  /*default_value=*/Varying(int(0)),
                                  /*only_one_allowed=*/true,
                                  /*requires_value=*/false);

  this->registerNewCLA(cli0);
  this->registerNewCLA(cli1);
  this->registerNewCLA(cli2);
  this->registerNewCLA(cli3);
  this->registerNewCLA(cli4);
}

// ###################################################################
void ModuleBase::parseCommandLine(int argc, char** argv)
{
  //=================================== Converts c-strings to std::string
  std::vector<std::string> raw_clas;
  for (int c = 0; c < argc; ++c)
    raw_clas.push_back(argv[c]);

  this->parseCommandLine(raw_clas);
}

// ###################################################################
void ModuleBase::parseCommandLine(const std::vector<std::string>& args)
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
    auto& supplied_clas = m_supplied_command_line_arguments;
    if (not supplied_clas.has(cla_spec.m_name)) supplied_clas.add(cla_spec);
    else if (cla_spec.m_only_one_allowed)
      elkLogicalError("Command line argument --" + cla_spec.m_name + "/-" +
                      cla_spec.m_short_name + " is only allowed once.");

    //============================ Check if actual value for key is required
    if (cla_spec.m_requires_value)
    {
      if ((c + 1) >= argc)
        elkLogicalError("Command line argument --" + cla_spec.m_name + "/-" +
                        cla_spec.m_short_name +
                        " requires a value to be supplied but the keyword " +
                        "specifier is at the end of command line," +
                        " so a value cannot be deduced for it.");

      const auto& next_cla = args.at(c + 1);
      if ((next_cla.find("-") == 0 and next_cla.size() == 2) or
          next_cla.find("--") == 0)
        elkLogicalError("Command line argument --" + cla_spec.m_name + "/-" +
                        cla_spec.m_short_name +
                        " requires a value to be supplied but the keyword " +
                        "is followed by another keyword specifier, " +
                        next_cla + ", therefore no value can be deduced.");

      const auto value = elke::Varying(next_cla);
      c += 1;
      auto& supplied_cla = supplied_clas.getCLAbyName(cla_spec.m_name);
      supplied_cla.m_values_assigned.push_back(value);
    }

  } // for int c
}

// ###################################################################
void ModuleBase::initialCLIResponse()
{
  const auto& supplied_clas = m_supplied_command_line_arguments;
  auto& logger = getLogger();

  this->printHeader();
  if (supplied_clas.has("help")) this->printHelp();

  if (supplied_clas.has("nocolor")) m_logger.setColorSuppression(true);

  if (supplied_clas.has("verbosity"))
  {
    const auto& cla = supplied_clas.getCLAbyName("verbosity");
    m_logger.setVerbosity(cla.m_values_assigned.front().IntegerValue());
  }

  if (supplied_clas.has("say-hello"))
    m_logger.log() << "Salutations from elk-e! \n";

  if (supplied_clas.has("dump-registry")) dumpRegistry();

  if (supplied_clas.has("basic"))
  {
    const auto& basic_commands_CLA = supplied_clas.getCLAbyName("basic");
    const auto& basic_commands = basic_commands_CLA.m_values_assigned;

    if (not basic_commands.empty())
      logger.log() << "Performing basic commands from CLI...\n";

    for (const auto& basic_command : basic_commands)
    {
      const std::string command = basic_command.StringValue();
      logger.log() << command << "\n";
      const auto& words = elke::string_utils::splitString(command);

      if (words.size() > 0 and words[0] == "call")
        this->basicCommandCall(command);
      else
      {
        const std::string error_message =
          std::string(__PRETTY_FUNCTION__) + ":\n" +
          "Command Line Argument --basic/-b processes "
          "only commands that start with 'call'."
          " Unknown command '" +
          words[0] + "'.";
        logger.errorAllRanks() << error_message;
        elke::Abort("CLA processing.");
      }
    }
  } // if (supplied_clas.has("basic"))
}

// ###################################################################
void ModuleBase::printHelp()
{
  const auto& supplied_clas = this->m_supplied_command_line_arguments;
  const auto& registered_clas = this->m_registered_command_line_arguments;

  std::stringstream outstr;

  outstr << "Command line arguments specification:\n";
  for (const auto& cla : registered_clas.getCLAlist())
  {
    const std::string short_part =
      (cla.m_short_name.empty()) ? "" : ", -" + cla.m_short_name;
    const auto column1 = "--" + cla.m_name + short_part;
    const int offset = 20 - column1.size();

    outstr << column1;
    if (offset < 0) outstr << "\n" + std::string(20, ' ');
    else
      outstr << std::string(offset, ' ');

    outstr << cla.m_doc_string;

    outstr << "\n";
  }
  outstr << "\n";

  getLogger().log() << outstr.str();
}

// ###################################################################
void ModuleBase::printHeader()
{
  const auto& supplied_clas = m_supplied_command_line_arguments;

  std::stringstream outstr;

  outstr << m_program_header << "\n";
  outstr << "Version: " << PROJECT_VERSION << "\n\n";

  const auto& supplied_clas_list = supplied_clas.getCLAlist();
  if (not supplied_clas_list.empty())
    outstr << "Valid command line arguments supplied:\n";
  else
    outstr << m_logger.stringColor(FG_YELLOW) +
                "No command line arguments have been supplied.\n" +
                m_logger.stringColor(RESET);

  for (const auto& cla : supplied_clas_list)
  {
    const std::string short_part =
      (cla.m_short_name.empty()) ? "" : ", -" + cla.m_short_name;
    const auto column1 = "--" + cla.m_name + short_part;
    const int offset = 20 - column1.size();

    outstr << column1 + std::string(offset, ' ');
    for (const auto& val : cla.m_values_assigned)
    {
      outstr << val.StringValue();
      if (&val != &*cla.m_values_assigned.rbegin()) outstr << ", ";
    }
    outstr << "\n";
  }
  outstr << "\n";

  getLogger().log() << outstr.str();
}

// ###################################################################
void ModuleBase::dumpRegistry()
{
  const auto& registry = Registry::getInstance();
  const auto& module_registry = registry.getModuleRegistry();
  m_logger.log() << "Dumping the registry" << module_registry.size() << "\n";
  for (const auto& entry : module_registry)
  {
    m_logger.log() << entry.name << ":\n";
    ModuleBase& module_inst = entry.fetcher();

    m_logger.log() << "  nullary_functions:\n";
    const auto& nullary_functions = module_inst.getNullaryFunctions();
    for (const auto& [name, _] : nullary_functions)
      m_logger.log() << "    " << name << "\n";

  } // for module
}

// ###################################################################
void ModuleBase::basicCommandCall(const std::string& command_string)
{
  auto& logger = m_logger;
  const auto& words = elke::string_utils::splitString(command_string);

  //=================================== Check sufficient words
  if (words.size() < 2)
  {
    std::stringstream ostr;
    ostr << std::string(__PRETTY_FUNCTION__) +
              ":\nAt least two words are required for --basic/-b command line "
              "arguments."
              " Words processed: '";
    for (const auto& word : words)
    {
      ostr << word;
      if (word != words.back()) ostr << ",";
    }
    ostr << "'";
    logger.errorAllRanks() << ostr.str();
  }

  //=================================== Search function
  const auto& function_name = words[1];
  const auto& registry = elke::Registry::getInstance();
  const auto& module_registry = registry.getModuleRegistry();

  for (const auto& module_entry : module_registry)
  {
    ModuleBase& module_inst = module_entry.fetcher();
    const auto& nullary_functions = module_inst.getNullaryFunctions();
    for (const auto& [name, function] : nullary_functions)
      if (name == function_name) function();
  }
}

} // namespace elke
