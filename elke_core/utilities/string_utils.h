#ifndef ELKE_CORE_UTILITIES_STRING_UTILS_H
#define ELKE_CORE_UTILITIES_STRING_UTILS_H

#include <string>
#include <vector>

namespace elke::string_utils
{

/**Split a string given a set of delimiters.*/
std::vector<std::string>
splitString(const std::string& input,
            const std::string& delimiter = " ",
            bool treat_consecutive_delimiters_as_one = true);

} // namespace elke::string_utils

#endif // ELKE_CORE_UTILITIES_STRING_UTILS_H
