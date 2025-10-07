#include "LogStream.h"
#include "StringColor.h"

namespace elke
{

/**Creates a string stream.*/
LogStream::LogStream(std::ostream* output_stream,
                     std::string header,
                     bool dummy_flag /*=false*/)
  : m_log_stream(output_stream), m_log_header(header), m_dummy(dummy_flag)
{
}

/**Flushes the headered stream to the output.*/
LogStream::~LogStream()
{
  if (m_dummy) return;

  std::string line, oline;
  while (std::getline(*this, line))
    oline += m_log_header + line + '\n' + std::string("\033[") +
             std::to_string(StringColorCode::RESET) + "m";

  if (!oline.empty()) *m_log_stream << oline << std::flush;
}

/**Initializes the stream via another. */
LogStream::LogStream(const LogStream& other)
{
  m_log_stream = other.m_log_stream;
  m_log_header = other.m_log_header;
}

} // namespace elke
