#ifndef ELKE_CORE_OUTPUT_LOGGER_H
#define ELKE_CORE_OUTPUT_LOGGER_H

#include "LogStream.h"
#include "StringColor.h"

/**A singleton class to handle multiprocess logging.*/
namespace elke
{

enum LogVerbosity
{
  LEVEL_1 = 1,
  LEVEL_2 = 2,
  LEVEL_3 = 3
};

class Logger
{
public:
  Logger(int verbosity);
  static Logger& getInstance() noexcept;
  int getVerbosity() {return m_verbosity;}
  LogStream log(LogVerbosity verbosity = LogVerbosity::LEVEL_1);
  LogStream logAllRanks(LogVerbosity verbosity = LogVerbosity::LEVEL_1);
  LogStream warn(LogVerbosity verbosity = LogVerbosity::LEVEL_1);
  LogStream warnAllRanks(LogVerbosity verbosity = LogVerbosity::LEVEL_1);
  LogStream error(LogVerbosity verbosity = LogVerbosity::LEVEL_1);
  LogStream errorAllRanks(LogVerbosity verbosity = LogVerbosity::LEVEL_1);

  std::string stringColor(StringColorCode code);

  void setColorSuppression(bool value);
  void setVerbosity(int verbosity);

private:
  DummyStream m_dummy_stream;
  int m_verbosity = 1;
  bool m_suppress_color = false;
};

} // namespace elke

#endif // ELKE_CORE_OUTPUT_LOGGER_H
