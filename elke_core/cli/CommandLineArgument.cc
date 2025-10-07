#include "CommandLineArgument.h"

namespace elke
{

CommandLineArgument::CommandLineArgument(const std::string& name,
                                         const std::string& short_name,
                                         const std::string& doc_string,
                                         const elke::Varying& default_value,
                                         bool only_one_allowed/*=true*/,
                                         bool requires_value/*=true*/)
  : m_name(name),
    m_short_name(short_name),
    m_doc_string(doc_string),
    m_default_value(default_value),
    m_only_one_allowed(only_one_allowed),
    m_requires_value(requires_value)
{
}
} // namespace elk

std::ostream& operator<<(std::ostream& outstr,
                         const elke::CommandLineArgument& arg)
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
