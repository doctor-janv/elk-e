#ifndef ELKE_CORE_UTILITIES_GENERAL_UTILS_H
#define ELKE_CORE_UTILITIES_GENERAL_UTILS_H

#include <string>
#include <vector>

namespace elke
{

struct WarningsAndErrorsData
{
  std::vector<std::string> m_warnings;
  std::vector<std::string> m_errors;
};

void Abort(const std::string& program_phase);

} // namespace elke

#endif // ELKE_CORE_UTILITIES_GENERAL_UTILS_H
