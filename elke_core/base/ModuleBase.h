#ifndef ELKE_CORE_BASES_MODULEBASE_H
#define ELKE_CORE_BASES_MODULEBASE_H

#include "elke_core/output/Logger.h"
#include "elke_core/cli/CommandLineArgument.h"
#include "elke_core/cli/CommandLineArgumentList.h"
#include "elke_core/registration/registration.h"

#include <map>
#include <string>
#include <vector>

namespace elke
{

/**A base module class.*/
class ModuleBase
{
  std::map<std::string, NullaryFunction> m_nullary_function_register;
  Logger m_logger;

protected:
  const std::string m_name;
  std::string m_program_header;
  CommandLineArgumentList m_registered_command_line_arguments;
  CommandLineArgumentList m_supplied_command_line_arguments;
  bool m_suppress_color = false;
  int m_verbosity;

public:
  /**Registers a Nullary Function (no argument function). */
  void registerNullaryFunction(const std::string& function_name,
                               NullaryFunction function);

  /**Returns the logger.*/
  Logger& getLogger();

  void sayHi();

  const std::map<std::string, NullaryFunction>& getNullaryFunctions() const;

protected:
  explicit ModuleBase(const std::string& name);

  //01
  /**Registers a new CLA into the CLA-registry.*/
  void registerNewCLA(const CommandLineArgument& cla);

  /**Registers CLI.*/
  void registerCLI();

  /**Parses the command line arguments from raw c-style arguments.*/
  void parseCommandLine(int argc, char** argv);
  void parseCommandLine(const std::vector<std::string>& args);

  /**Initial CLI response.*/
  void initialCLIResponse();
  void printHelp();
  void printHeader();
  void dumpRegistry();
  void basicCommandCall(const std::string& command_string);

private:
  /**Sets the default program header.*/
  static std::string setDefaultHeader();
};

} // namespace elke

#endif // ELKE_CORE_BASES_MODULEBASE_H
