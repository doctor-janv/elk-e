#ifndef ELK_GLOBALS_H
#define ELK_GLOBALS_H

/**Small utility macro for joining two words.*/
#define ElkeJoinWordsA(x, y) x##y
/**IDK why this is needed. Seems like counter doesnt work properly without it*/
#define ElkeJoinWordsB(x, y) ConsoleJoinWordsA(x, y)

#define RegisterGlobalCLA(cla)                                                 \
  static char ElkeJoinWordsB(unique_var_name_clas, __COUNTER__) =              \
    elk::Globals::RegisterCLA(cla)

#include <vector>
#include <string>

#include "elke_core/cli/CommandLineArgumentList.h"

namespace elk
{

struct CommandLineArgument;
class CommandLineArgumentList;

class Globals
{
public:
  /**Returns a reference to the singleton.*/
  static Globals& getInstance() noexcept;

  /**Returns a const reference to the Command Line Arguments specification.*/
  const CommandLineArgumentList& getRegisteredCLAs() const;

  /**Returns a non-const reference to the Command Line Arguments supplied.*/
  CommandLineArgumentList& getSuppliedCLAs();

  /**Returns a const reference to the Command Line Arguments supplied.*/
  const CommandLineArgumentList& getSuppliedCLAs() const;

  /**Puts a new CLA into the CLA-list.*/
  static void registerCLI(const CommandLineArgument& cla);

  const std::string& getProgramHeader() const;

  void changeProgramHeader(const std::string& new_header);

private:
  Globals() noexcept;

private:
  CommandLineArgumentList m_registered_command_line_arguments;
  CommandLineArgumentList m_supplied_command_line_arguments;
  // clang-format off
  std::string m_program_header =
"*******************************************************************************\n"
"*                                 ELKE                                        *\n"
"*                    A computational physics library                          *\n"
"*                          for thermal-hydraulics                             *\n"
"*******************************************************************************\n";
  // clang-format on

  static Globals m_instance;
};

} // namespace elk

#endif // ELK_GLOBALS_H
