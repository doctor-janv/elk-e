#ifndef ELKE_CORE_OUTPUT_LOGGER_H
#define ELKE_CORE_OUTPUT_LOGGER_H

#include "LogStream.h"
#include "StringColor.h"

/**A singleton class to handle multiprocess logging.*/
namespace elke
{

enum class LogVerbosity
{
  LEVEL_1 = 1,
  LEVEL_2 = 2,
  LEVEL_3 = 3
};

class Logger
{
  DummyStream m_dummy_stream;
  const int m_rank;
  int m_verbosity = 1;
  bool m_suppress_color = false;

public:
  explicit Logger(int verbosity, int rank);
  int getVerbosity() const {return m_verbosity;}
  LogStream log(LogVerbosity verbosity = LogVerbosity::LEVEL_1);
  LogStream logAllRanks(LogVerbosity verbosity = LogVerbosity::LEVEL_1) const;
  LogStream warn(LogVerbosity verbosity = LogVerbosity::LEVEL_1);
  LogStream warnAllRanks(LogVerbosity verbosity = LogVerbosity::LEVEL_1) const;
  LogStream error(LogVerbosity verbosity = LogVerbosity::LEVEL_1);
  LogStream errorAllRanks(LogVerbosity verbosity = LogVerbosity::LEVEL_1) const;

  std::string stringColor(StringColorCode code) const;

  void setColorSuppression(bool value);
  void setVerbosity(int verbosity);
};

} // namespace elke

#endif // ELKE_CORE_OUTPUT_LOGGER_H
