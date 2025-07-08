#include "CommandLineArgument.h"

namespace elk
{

CommandLineArgument::CommandLineArgument(const std::string& name,
                                         const std::string& short_name,
                                         const std::string& doc_string)

  : m_name(name),
    m_short_name(short_name),
    m_doc_string(doc_string),
    m_default_value(0)
{
}
CommandLineArgument::CommandLineArgument(const std::string& name,
                                         const std::string& short_name,
                                         const std::string& doc_string,
                                         const elk::Varying& default_value)
  : m_name(name),
    m_short_name(short_name),
    m_doc_string(doc_string),
    m_default_value(default_value)
{
}
} // namespace elk

std::ostream& operator<<(std::ostream& outstr,
                         const elk::CommandLineArgument& arg)
{
  const std::string column1 = "--" + arg.m_name + ", -" + arg.m_short_name;

  outstr << column1;

  const int offset = 20 - column1.size();
  if (offset < 2) outstr << "\n" << std::string(20, ' ');
  else
    outstr << std::string(offset, ' ');

  outstr << arg.m_doc_string << "\n";

  return outstr;
}
