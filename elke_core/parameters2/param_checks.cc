#include "param_checks.h"

#include <sstream>

namespace elke::param_checks
{

bool ArraySizeCheck::performCheck(StatusStrings& status_strings,
                                  const DataTree& data,
                                  const std::string& prefix,
                                  const std::string& name_or_id) const
{
  const size_t data_num_children = data.numChildren();
  if (data_num_children != m_size)
  {
    std::stringstream error_message;
    // clang-format off
    error_message
      << prefix << "Item \"" << name_or_id << "\" is required to be an array "
      << "of size " << m_size << " but the data provided has "
      << data_num_children << " entries.\n";
    // clang-format on

    status_strings.m_errors.append(error_message.str());
    return false;
  }

  return true;
}

} // namespace elke::param_checks