#include "general_utils.h"

#include <exception>

namespace elke
{

void Abort(const std::string& program_phase)
{
  throw std::logic_error("\n\nFailure during " + program_phase + "\n");
}

} // namespace elke
