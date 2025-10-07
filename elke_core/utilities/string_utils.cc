#include "string_utils.h"

namespace elke::string_utils
{

/**Split a string given a set of delimiters.*/
std::vector<std::string>
splitString(const std::string& input,
            const std::string& delimiter /* = " " */,
            bool treat_consecutive_delimiters_as_one /* = true */)
{
  std::vector<std::string> output;
  const size_t input_len = input.size();
  const size_t delimiter_len = delimiter.size();
  typedef std::pair<size_t, size_t> DelimiterCoord;
  const size_t infty = std::string::npos;

  //============================================= Establish delimiters
  // First we find and store other the delimiter locations
  std::vector<DelimiterCoord> delimiter_locations;
  size_t last_pos = 0;
  while (true)
  {
    size_t find_pos = input.find(delimiter, last_pos);
    if (find_pos == infty) break;
    else
    {
      last_pos = find_pos + delimiter_len;
      delimiter_locations.push_back({find_pos, last_pos});
    }
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
        delimiter_locations.push_back({del_begin, del_end});
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
    if (last_pos == 0 and word_length == 0) /*pass*/;
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

} // namespace elke::string_utils
