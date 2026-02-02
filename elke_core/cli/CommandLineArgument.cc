#include "CommandLineArgument.h"

namespace elke
{

CommandLineArgument::CommandLineArgument(const std::string& name,
                                         const std::string& short_name,
                                         const std::string& doc_string,
                                         const elke::ScalarValue& default_value,
                                         const bool only_one_allowed/*=true*/,
                                         const bool requires_value/*=true*/)
  : m_name(name),
    m_short_name(short_name),
    m_doc_string(doc_string),
    m_default_value(default_value),
    m_only_one_allowed(only_one_allowed),
    m_requires_value(requires_value)
{
}
} // namespace elke

