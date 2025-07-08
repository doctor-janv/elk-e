#ifndef COMMAND_LINE_ARGUMENT_H
#define COMMAND_LINE_ARGUMENT_H

#include <vector>
#include <string>

#include "elke_core/data_types/Varying.h"

namespace elk
{

//**Base class for a command line argument.*/
struct CommandLineArgument
{
  /**Constructor. Will assign int(0) to default value.*/
  CommandLineArgument(const std::string& name,
                      const std::string& short_name,
                      const std::string& doc_string);
  /**Constructor with default value.*/
  CommandLineArgument(const std::string& name,
                      const std::string& short_name,
                      const std::string& doc_string,
                      const elk::Varying& default_value);

  std::string m_name;
  std::string m_short_name;
  std::string m_doc_string;

  elk::Varying m_default_value;
  std::vector<elk::Varying> m_values_assigned;
  bool m_only_one_allowed = true;
  bool m_requires_value = true;
};

} // namespace elk

/**Stream operator*/
std::ostream& operator<<(std::ostream& outstr,
                         const elk::CommandLineArgument& value);

#endif // COMMAND_LINE_ARGUMENT_H
