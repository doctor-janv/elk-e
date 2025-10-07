#include "CommandLineArgumentList.h"
#include "CommandLineArgument.h"
#include "elke_core/output/elk_exceptions.h"

namespace elke
{

void CommandLineArgumentList::add(const CommandLineArgument& arg)
{
  m_list.push_back(arg);
}

bool CommandLineArgumentList::has(const std::string& name) const
{
  for (const auto& cla : m_list)
    if (cla.m_name == name) return true;

  return false;
}

bool CommandLineArgumentList::hasShortName(const std::string& short_name) const
{
  for (const auto& cla : m_list)
    if (cla.m_short_name == short_name) return true;

  return false;
}

const CommandLineArgument&
CommandLineArgumentList::getCLAbyName(const std::string& name) const
{
  for (const auto& cla : m_list)
    if (cla.m_name == name) return cla;

  elkLogicalError("No CLA named " + name);
}
CommandLineArgument&
CommandLineArgumentList::getCLAbyName(const std::string& name)
{
  for (auto& cla : m_list)
    if (cla.m_name == name) return cla;

  elkLogicalError("No CLA named " + name);
}

const CommandLineArgument&
CommandLineArgumentList::getCLAbyShortName(const std::string& short_name) const
{
  for (const auto& cla : m_list)
    if (cla.m_short_name == short_name) return cla;

  elkLogicalError("No CLA with short name " + short_name);
}

const std::vector<CommandLineArgument>&
CommandLineArgumentList::getCLAlist() const
{
  return m_list;
}

} // namespace elke
