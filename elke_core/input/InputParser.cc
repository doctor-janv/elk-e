#include "InputParser.h"

namespace elke
{

 InputParser::InputParser(elke::Logger& logger) : m_logger(logger) {  }


const std::vector<std::string>& InputParser::errors() const { return m_errors; }
const std::vector<std::string>& InputParser::warnings() const
{
  return m_warnings;
}

} // namespace elke