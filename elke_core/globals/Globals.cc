#include "Globals.h"
#include "elke_core/cli/CommandLineArgument.h"
#include "elke_core/output/elk_exceptions.h"

namespace elk
{

Globals::Globals() noexcept {}

Globals& Globals::getInstance() noexcept
{
  static Globals singleton;
  return singleton;
}

const CommandLineArgumentList& Globals::getRegisteredCLAs() const
{
  return m_registered_command_line_arguments;
}

CommandLineArgumentList& Globals::getSuppliedCLAs()
{
  return m_supplied_command_line_arguments;
}
const CommandLineArgumentList& Globals::getSuppliedCLAs() const
{
  return m_supplied_command_line_arguments;
}

void Globals::registerCLI(const elk::CommandLineArgument& cla)
{
  auto& self = getInstance();

  elkInvalidArgumentIf(self.m_registered_command_line_arguments.has(cla.m_name),
                       "CLA with name \"" + cla.m_name +
                         "\" already registered");

  self.m_registered_command_line_arguments.add(cla);
}

const std::string& Globals::getProgramHeader() const
{
  return m_program_header;
}

void Globals::changeProgramHeader(const std::string& new_header)
{
  m_program_header = new_header;
}

} // namespace elk
