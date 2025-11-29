#ifndef ELK_E_COMMANDLINEINTERFACE_H
#define ELK_E_COMMANDLINEINTERFACE_H

#include "CommandLineArgumentList.h"
#include "elke_core/output/Logger.h"

namespace elke
{

class FrameworkCore;

class CommandLineInterface
{
  CommandLineArgumentList m_registered_command_line_arguments;
  CommandLineArgumentList m_supplied_command_line_arguments;
  std::string m_program_header;
  std::shared_ptr<elke::Logger> m_logger_ptr;

  // Interface
public:
  /**Registers a new CLA into the CLA-registry.*/
  void registerNewCLA(const CommandLineArgument& cla);
  /**Returns registered CLAs.*/
  const CommandLineArgumentList& getRegisteredCommandLineArguments() const;
  /**Returns supplied CLAs*/
  const CommandLineArgumentList& getSuppliedCommandLineArguments() const;
  /**Parses the command line arguments from raw c-style arguments.*/
  void parseCommandLine(int argc, char** argv);
  /**Parses c++ style command line arguments.*/
  void parseCommandLine(const std::vector<std::string>& args);

  /**Registers the items of the CLI shared among children.*/
  void registerCommonCLI_Items();

  /**Initial CLI response.*/
  void respondToBasicCLAs() const;

  const std::string& getHeader() const;
  void setHeader(std::string new_header);

protected:
  friend class elke::FrameworkCore;
  // Constructor/Destructor
  explicit CommandLineInterface(std::shared_ptr<elke::Logger> logger_ptr);

private:
  /**Sets the default program header.*/
  static std::string constructionHelperSetDefaultHeader();

  void printHelp() const;
  void printHeader() const;
};

} // namespace elke

#endif // ELK_E_COMMANDLINEINTERFACE_H
