#include "Logger.h"
#include "elke_core/cli/CommandLineArgument.h"
#include "StringColor.h"

namespace elke
{

Logger::Logger(const int verbosity, const int rank)
  : m_rank(rank), m_verbosity(verbosity)
{
}

LogStream Logger::log(LogVerbosity verbosity /*=LogVerbosity::LEVEL_1*/)
{
  if (m_rank == 0)
  {
    std::string header = "[0]  "; // Not yet modified for MPI
    return {&std::cout, header};
  }

  std::string header = " ";
  return {&m_dummy_stream, header};
}
LogStream
Logger::logAllRanks(LogVerbosity verbosity /*=LogVerbosity::LEVEL_1*/) const
{
  std::string header =
    "[" + std::to_string(m_rank) + "]  "; // Not yet modified for MPI
  return {&std::cout, header};
}

LogStream Logger::warn(LogVerbosity verbosity /*=LogVerbosity::LEVEL_1*/)
{
  if (m_rank == 0)
  {
    std::string header = this->stringColor(StringColorCode::FG_YELLOW) +
                         "[0]  WARNING: "; // Not yet modified for MPI
    return {&std::cout, header};
  }

  std::string header = " ";
  return {&m_dummy_stream, header};
}
LogStream
Logger::warnAllRanks(LogVerbosity verbosity /*=LogVerbosity::LEVEL_1*/) const
{
  std::string header = this->stringColor(StringColorCode::FG_YELLOW) + "[" +
                       std::to_string(m_rank) + "]  WARNING: ";
  return {&std::cout, header};
}

LogStream Logger::error(LogVerbosity verbosity /*=LogVerbosity::LEVEL_1*/)
{
  if (m_rank == 0)
  {
    std::string header = this->stringColor(StringColorCode::FG_RED) +
                         "[0]  ERROR: "; // Not yet modified for MPI
    return {&std::cout, header};
  }

  std::string header = " ";
  return {&m_dummy_stream, header};
}
LogStream
Logger::errorAllRanks(LogVerbosity verbosity /*=LogVerbosity::LEVEL_1*/) const
{
  std::string header = this->stringColor(StringColorCode::FG_RED) + "[" +
                       std::to_string(m_rank) +
                       "]  ERROR: "; // Not yet modified for MPI
  return {&std::cout, header};
}

std::string Logger::stringColor(const StringColorCode code) const
{
  if (m_suppress_color) return {};

  return std::string("\033[") + std::to_string(code) + "m";
}

void Logger::setColorSuppression(bool value) { m_suppress_color = value; }

void Logger::setVerbosity(int verbosity)
{
  if (verbosity < 1) m_verbosity = 1;
  else if (verbosity > 3)
    m_verbosity = 3;
  else
    m_verbosity = verbosity;
}

} // namespace elke
