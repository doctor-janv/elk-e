#ifndef ELK_EXCEPTIONS_H
#define ELK_EXCEPTIONS_H

#include <string>
#include <stdexcept>

#define elkInvalidArgumentIf(condition, message)                               \
  if (condition)                                                               \
  throw std::invalid_argument(std::string(__PRETTY_FUNCTION__) + ":\n" +        \
                              message)
#define elkInvalidArgument(message)                                            \
  throw std::invalid_argument(std::string(__PRETTY_FUNCTION__) + ":\n" +        \
                              message)

#define elkLogicalErrorIf(condition, message)                                  \
  if (condition)                                                               \
  throw std::logic_error(std::string(__PRETTY_FUNCTION__) + ":\n" + message)

#define elkLogicalError(message)                                               \
  throw std::logic_error(std::string(__PRETTY_FUNCTION__) + ":\n" + message)

#endif // ELK_EXCEPTIONS_H
