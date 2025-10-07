#ifndef COMMAND_LINE_ARGUMENT_H
#define COMMAND_LINE_ARGUMENT_H

#include <vector>
#include <string>

#include "elke_core/data_types/Varying.h"

namespace elke
{

//**Base class for a command line argument.*/
struct CommandLineArgument
{
  CommandLineArgument(const std::string& name,
                      const std::string& short_name,
                      const std::string& doc_string,
                      const elke::Varying& default_value = Varying(int(0)),
                      bool only_one_allowed = true,
                      bool requires_value = true);

  const std::string m_name;
  const std::string m_short_name;
  const std::string m_doc_string;

  const elke::Varying m_default_value;
  const bool m_only_one_allowed;
  const bool m_requires_value;
  std::vector<elke::Varying> m_values_assigned;
};

} // namespace elke

/**Stream operator*/
std::ostream& operator<<(std::ostream& outstr,
                         const elke::CommandLineArgument& value);

#endif // COMMAND_LINE_ARGUMENT_H
