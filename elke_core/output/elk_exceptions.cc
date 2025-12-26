#include "elk_exceptions.h"
#include "elke_core/FrameworkCore.h"

namespace elke::exceptions
{

InvalidArgument::InvalidArgument(const std::string& message)
  : std::runtime_error(message + FrameworkCore::getStacktrace())
{
}

LogicError::LogicError(const std::string& message)
  : std::runtime_error(message + FrameworkCore::getStacktrace())
{
}

} // namespace elke::exceptions