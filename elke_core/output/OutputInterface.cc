#include "OutputInterface.h"

namespace elke
{

OutputInterface::OutputInterface(int rank)
  : m_logger_ptr(std::make_shared<Logger>(/*verbosity=*/1, /*rank=*/rank))
{
}

Logger& OutputInterface::getLogger() const { return *m_logger_ptr; }

std::shared_ptr<Logger> OutputInterface::getLoggerPtr() const
{
  return m_logger_ptr;
}

} // namespace elke