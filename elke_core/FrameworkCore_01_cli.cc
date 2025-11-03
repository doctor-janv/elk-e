#include "FrameworkCore.h"

#include "elke_core/cli/CommandLineArgument.h"

#include "elke_core/utilities/string_utils.h"
#include "elke_core/utilities/general_utils.h"

namespace elke
{

// ###################################################################
/**Registers CLI items specific to the CoreModule.*/
void FrameworkCore::registerFrameworkCoreSpecificCLI()
{
  const auto cli0 = CommandLineArgument(
    "module",
    "m",
    "Sets the name of the module that will dictate the execution",
    /*default_value=*/Varying("NullModule"),
    /*only_one_allowed=*/true,
    /*requires_value=*/false);

  const auto cli1 = CommandLineArgument("dump-registry",
                                        "",
                                        "Dumps the registry in yaml format",
                                        /*default_value=*/Varying(0),
                                        /*only_one_allowed=*/true,
                                        /*requires_value=*/false);

  const auto cli2 = CommandLineArgument("basic",
                                        "b",
                                        "Calls a basic command in the "
                                        "form 'command arg1 arg2 arg3 etc.'",
                                        /*default_value=*/Varying(""),
                                        /*only_one_allowed=*/false,
                                        /*requires_value=*/true);

  this->registerNewCLA(cli0);
  this->registerNewCLA(cli1);
  this->registerNewCLA(cli2);
}

// ###################################################################
void FrameworkCore::respondToFrameworkCoreCLAs() const
{
  const auto& supplied_clas = getSuppliedCommandLineArguments();
  auto& logger = this->getLogger();

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
      const auto& words = string_utils::splitString(command);

      if (not words.empty() and words[0] == "call")
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
void FrameworkCore::basicCommandCall(const std::string& command_string) const
{
  auto& logger = this->getLogger();
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

  bool function_found = false;
  for (const auto& [name, function] : m_nullary_function_register)
  {
    if (name == function_name)
    {
      function();
      function_found = true;
    }
  }
  if (!function_found)
  {
    logger.error() << "Nullary function '" << function_name
                   << "' not found amount registered nullary functions.";
  }
}

// ###################################################################
void FrameworkCore::dumpRegistry() const
{
  auto& logger = this->getLogger();

  const auto& nullary_functions = m_nullary_function_register;

  logger.log() << "nullary_functions:"
               << (nullary_functions.empty() ? " []" : "\n");

  for (const auto& [name, _] : nullary_functions)
    logger.log() << name << "\n";
}

} // namespace elke
