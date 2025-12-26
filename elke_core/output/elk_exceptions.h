#ifndef ELK_EXCEPTIONS_H
#define ELK_EXCEPTIONS_H

#include <string>
#include <stdexcept>

#define elkInvalidArgumentIf(condition, message)                               \
  if (condition)                                                               \
  throw elke::exceptions::InvalidArgument(std::string(__PRETTY_FUNCTION__) +    \
                                         ":\n" + message)
#define elkInvalidArgument(message)                                            \
  throw elke::exceptions::InvalidArgument(std::string(__PRETTY_FUNCTION__) +    \
                                         ":\n" + message)

#define elkLogicalErrorIf(condition, message)                                  \
  if (condition)                                                               \
  throw elke::exceptions::LogicError(std::string(__PRETTY_FUNCTION__) + ":\n" + \
                                    message)

#define elkLogicalError(message)                                               \
  throw elke::exceptions::LogicError(std::string(__PRETTY_FUNCTION__) + ":\n" + \
                                    message)

namespace elke::exceptions
{

class InvalidArgument final : public std::runtime_error
{
public:
  explicit InvalidArgument(const std::string& message);
};

class LogicError final: public std::runtime_error
{
public:
  explicit LogicError(const std::string& message);
};

} // namespace elke::exceptions

#endif // ELK_EXCEPTIONS_H
