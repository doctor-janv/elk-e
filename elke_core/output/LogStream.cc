#include "LogStream.h"

#include <utility>
#include "StringColor.h"

namespace elke
{

/**Creates a string stream.*/
LogStream::LogStream(std::ostream* output_stream,
                     std::string header,
                     const bool& suppress_color)
  : m_log_stream(output_stream),
    m_log_header(std::move(header)),
    m_suppress_color(suppress_color)
{
}

/**Flushes the headered stream to the output.*/
LogStream::~LogStream()
{
  std::string line, oline;
  while (std::getline(*this, line))
  {
    oline += m_log_header + line + '\n';
    if (not m_suppress_color)
      oline +=
        std::string("\033[") + std::to_string(StringColorCode::RESET) + "m";
  }

  if (!oline.empty()) *m_log_stream << oline << std::flush;
}

/**Initializes the stream via another. */
LogStream::LogStream(const LogStream& other) :
m_log_stream(other.m_log_stream),
m_log_header(other.m_log_header),
m_suppress_color(other.m_suppress_color)
{
}

} // namespace elke
