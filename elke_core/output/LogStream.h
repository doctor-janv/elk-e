#ifndef ELKE_CORE_OUTPUT_LOGSTREAM_H
#define ELKE_CORE_OUTPUT_LOGSTREAM_H

#include <iostream>
#include <sstream>

namespace elke
{

// ###################################################################
/** Log stream for adding header information to a string stream.*/
class LogStream : public std::stringstream
{
public:
  /**Creates a string stream.*/
  LogStream(std::ostream* output_stream,
            std::string header,
            const bool& color_active);

  /**Flushes the headered stream to the output.*/
  ~LogStream() override;

  /**Initializes the stream via another. */
  LogStream(const LogStream& other);

private:
  std::ostream* m_log_stream;
  std::string m_log_header;
  const bool& m_color_active = false;
};

// ###################################################################
/**A dummy stream class to inhibit output from a particular rank.*/
struct DummyStream: std::ostream
{
  struct DummyStreamBuffer final : std::streambuf
  {
    int overflow(const int c) override { return c; }
  } buffer;

  DummyStream(): std::ostream(&buffer) {}

  ~DummyStream() override = default;
};

} // namespace elke

#endif // ELKE_CORE_OUTPUT_LOGSTREAM_H
