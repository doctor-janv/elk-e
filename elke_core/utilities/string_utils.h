#ifndef ELKE_CORE_UTILITIES_STRING_UTILS_H
#define ELKE_CORE_UTILITIES_STRING_UTILS_H

#include <string>
#include <vector>
#include <unordered_set>

namespace elke::string_utils
{

/**Split a string given a set of delimiters.*/
std::vector<std::string>
splitString(const std::string& input,
            const std::string& delimiter = " ",
            bool treat_consecutive_delimiters_as_one = true);

/**Determines if a vector of strings has a particular string.*/
bool stringListHasString(const std::vector<std::string>& string_list,
                         const std::string& target_string);

/**Determines the closest matching string in a list.*/
std::string
findClosestMatchingString(const std::string& input,
                          const std::vector<std::string>& list,
                          unsigned int distance_threshold = 4);

/**Function to calculate Levenshtein distance. */
unsigned int computeLevenshteinDistance(const std::string& s1,
                                        const std::string& s2);

/**Determines if a string is a number.*/
bool isStringANumber(const std::string& input);

/**Convert a string to int64_t.
 *
 * \warning The convertibility should be checked with isStringANumber.
 */
int64_t convertStringToInt64_t(const std::string& input);

/**Convert a string to double.
 *
\warning The convertibility should be checked with isStringANumber.
 */
int64_t convertStringToDouble(const std::string& input);

} // namespace elke::string_utils

#endif // ELKE_CORE_UTILITIES_STRING_UTILS_H
