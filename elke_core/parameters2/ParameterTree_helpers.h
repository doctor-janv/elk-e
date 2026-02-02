#ifndef ELK_E_PARAMETERTREE_HELPERS_H
#define ELK_E_PARAMETERTREE_HELPERS_H

namespace elke
{

using AddtionalInputCheckPtr = std::unique_ptr<param_checks::InputCheck>;

/**Helper class to identify an array of arbitrary types.*/
class GenericParameterArray
{
  std::vector<DataTree> m_default_values;

public:
  GenericParameterArray() = default;
  explicit GenericParameterArray(const std::vector<DataTree>& values)
    : m_default_values(values)
  {
  }
  const std::vector<DataTree>& defaultValues() const
  {
    return m_default_values;
  }
  std::vector<DataTree>& defaultValues() { return m_default_values; }
};

/**Helper class to identify a map of arbitrary types.*/
class GenericParameterMap
{
  DataTree m_default_value;

public:
  GenericParameterMap() : m_default_value("Default") {}
  explicit GenericParameterMap(const DataTree& tree) : m_default_value(tree) {}

  const DataTree& defaultValue() const { return m_default_value; }
  DataTree& defaultValue() { return m_default_value; }
};

/**Structure used to build a list of AdditionalInputChecks.*/
struct AdditionalInputCheckEntry
{
  AddtionalInputCheckPtr m_check;
  bool m_allow_subsequent_checks = true;

  AdditionalInputCheckEntry(AddtionalInputCheckPtr check,
                            const bool allow_subsequent_checks)
    : m_check(std::move(check)),
      m_allow_subsequent_checks(allow_subsequent_checks)
  {
  }

  AdditionalInputCheckEntry(const AdditionalInputCheckEntry& other)
    : m_check(other.m_check->clone()),
      m_allow_subsequent_checks(other.m_allow_subsequent_checks)
  {
  }
};

} // namespace elke

#endif // ELK_E_PARAMETERTREE_HELPERS_H
