#include "string_utils.h"

namespace elke::string_utils
{

// ###################################################################
/**Split a string given a set of delimiters.*/
std::vector<std::string>
splitString(const std::string& input,
            const std::string& delimiter /* = " " */,
            const bool treat_consecutive_delimiters_as_one /* = true */)
{
  std::vector<std::string> output;
  const size_t input_len = input.size();
  const size_t delimiter_len = delimiter.size();
  typedef std::pair<size_t, size_t> DelimiterCoord;
  constexpr size_t infty = std::string::npos;

  //============================================= Establish delimiters
  // First we find and store other the delimiter locations
  std::vector<DelimiterCoord> delimiter_locations;
  size_t last_pos = 0;
  while (true)
  {
    size_t find_pos = input.find(delimiter, last_pos);
    if (find_pos == infty) break;
    // else
    last_pos = find_pos + delimiter_len;
    delimiter_locations.emplace_back(find_pos, last_pos);
  }

  //============================================= Merge delimiters (if needed)
  if (treat_consecutive_delimiters_as_one)
  {
    const auto original_delimiter_locations = delimiter_locations;
    delimiter_locations.clear();
    DelimiterCoord last_delimiter_location = {infty, infty};
    for (const auto& delimiter_location : original_delimiter_locations)
    {
      const auto& [del_begin, del_end] = delimiter_location;
      const auto& [last_del_begin, last_del_end] = last_delimiter_location;

      if (del_begin != last_del_end)
      {
        delimiter_locations.emplace_back(del_begin, del_end);
        last_delimiter_location = {del_begin, del_end};
      }
      else
      {
        delimiter_locations.back().second = del_end;
        last_delimiter_location = {last_del_begin, del_end};
      }
    }
  } // if (treat_consecutive_delimiters_as_one)

  //============================================= Extract words
  last_pos = 0;
  for (const auto& delimiter_location : delimiter_locations)
  {
    const auto& [del_start, del_end] = delimiter_location;

    const size_t word_length = del_start - last_pos;
    if (last_pos == 0 and word_length == 0) /*pass*/ {}
    else
      output.push_back(input.substr(last_pos, word_length));
    last_pos = del_end;

    if (delimiter_location == delimiter_locations.back())
      if (del_end != input_len)
        output.push_back(input.substr(del_end, input_len - del_end));
  }

  //============================================= Special case
  // Case where no delimiters are found
  if (delimiter_locations.empty() and input_len > 0) output.push_back(input);

  return output;
}

// ###################################################################
/**Determines if a vector of strings has a particular string.*/
bool stringListHasString(const std::vector<std::string>& string_list,
                         const std::string& target_string)
{
  for (const auto& string_in_list : string_list) // NOLINT(*-use-anyofallof)
    if (string_in_list == target_string) return true;

  return false;
}

// ###################################################################
/**Determines the closest matching string in a list.*/
std::string
findClosestMatchingString(const std::string& input,
                          const std::unordered_set<std::string>& dict,
                          const unsigned int distance_threshold /*=4*/)
{
  unsigned int min_dist = 1000000; // Large number
  std::string suggestion;

  for (const std::string& word : dict)
  {
    const unsigned int dist = computeLevenshteinDistance(input, word);
    // Only consider words within a threshold
    if (dist <= distance_threshold && dist < min_dist)
    {
      min_dist = dist;
      suggestion = word;
    }
  }
  return suggestion;
}

// ###################################################################
/**Function to calculate Levenshtein distance.
 * The Levenshtein distance is a string metric for measuring the difference
 * between two sequences. */
unsigned int computeLevenshteinDistance(const std::string& s1,
                                        const std::string& s2)
{
  using uint = unsigned int;
  const uint m = s1.length();
  const uint n = s2.length();

  // Create a 2D vector (matrix) to store the distances
  // The matrix size is (m+1) x (n+1)
  using RowOfUInt = std::vector<uint>;
  using MatrixOfUInt = std::vector<RowOfUInt>;
  MatrixOfUInt dp(m + 1, RowOfUInt(n + 1));

  //=================================== Initialize the first row and column
  // The distance from an empty string to a prefix of length i is i (i
  // insertions)
  for (int i = 0; i <= m; ++i)
    dp[i][0] = i;

  // The distance from a prefix of length j to an empty string is j (j
  // deletions)
  for (int j = 0; j <= n; ++j)
    dp[0][j] = j;

  //===================================  Fill the matrix
  for (int i = 1; i <= m; ++i)
  {
    for (int j = 1; j <= n; ++j)
    {
      // Calculate the cost of substitution
      // Cost is 0 if characters are the same, 1 if different
      const uint cost = s1[i - 1] == s2[j - 1] ? 0 : 1;

      // The value at dp[i][j] is the minimum of:
      // 1. Deletion: dp[i - 1][j] + 1
      // 2. Insertion: dp[i][j - 1] + 1
      // 3. Substitution: dp[i - 1][j - 1] + cost
      dp[i][j] = std::min({
        dp[i - 1][j] + 1,       // Deletion
        dp[i][j - 1] + 1,       // Insertion
        dp[i - 1][j - 1] + cost // Substitution
      });
    } // for j
  } // for i

  // The bottom-right cell contains the final Levenshtein distance
  return dp[m][n];
}

// ###################################################################
/**Determines if a string is a number.*/
bool isStringANumber(const std::string& input)
{
  try
  {
    // Attempt to convert the string to an integer
    std::size_t pos;
    std::stod(input, &pos);
    // Check if the entire string was consumed during conversion
    return pos == input.length();
  }
  catch (std::out_of_range const& ex)
  {
    // Number out of range for double
    return false;
  }
  catch (std::invalid_argument const& ex)
  {
    // String does not contain a valid number
    return false;
  }
}

// ###################################################################
/**Convert a string to int64_t.
 *
 * \warning The convertibility should be checked with isStringANumber.
 */
int64_t convertStringToInt64_t(const std::string& input)
{
  return std::stoll(input);
}

// ###################################################################
/**Convert a string to double.
 *
\warning The convertibility should be checked with isStringANumber.
 */
int64_t convertStringToDouble(const std::string& input)
{
  return std::stod(input);
}

} // namespace elke::string_utils
