#ifndef ELK_E_GENERIC_ITERATOR_H
#define ELK_E_GENERIC_ITERATOR_H

#include "template_helpers.h"

#include <vector>

namespace elke
{

// ###################################################################
template <typename VectorType>
class VectorOfPointersIterable
{
  std::vector<VectorType>& m_data;
  using PointedToType = typename VectorType::element_type;

public:
  explicit VectorOfPointersIterable(std::vector<VectorType>& data)
    : m_data(data)
  {
    static_assert(elke::is_smart_pointer_v<VectorType>,
                  "VectorType must be a std::unique_ptr or std::shared_ptr.");
  }

  /**A generic iterator for any std::vector with template type `VectorType`.*/
  class VectorOfPointersIterator
  {
    /// The vector of data on which this iterator operates.
    std::vector<VectorType>& m_data;
    /// The current index.
    size_t m_index;

  public:
    /// Constructor.
    VectorOfPointersIterator(std::vector<VectorType>& data, const size_t index)
      : m_data(data), m_index(index)
    {
    }

    /// De-reference operator.
    PointedToType& operator*() const { return *m_data[m_index]; }

    // clang-format off
    /// Prefixed increment operator.
    VectorOfPointersIterator operator++() { const VectorOfPointersIterator i = *this; m_index++; return i; }
    /// Post-fixed increment operator.
    VectorOfPointersIterator operator++(int) { m_index++; return *this; }

    /// Boolean equity operator
    bool operator==(const VectorOfPointersIterator& rhs) const { return m_index == rhs.m_index; }
    /// Boolean inequity operator
    bool operator!=(const VectorOfPointersIterator& rhs) const { return m_index != rhs.m_index; }
    // clang-format on
  };

protected:
  std::vector<VectorType>& data() { return m_data; }
  const std::vector<VectorType>& data() const { return m_data; }

public:
  // clang-format off
  VectorOfPointersIterator begin() { return {m_data, 0}; }
  VectorOfPointersIterator end() { return {m_data, m_data.size()}; }
  size_t size() const { return m_data.size(); }
  // clang-format on
};

// ###################################################################
template <typename VectorType>
class VectorOfPointersConstIterable
{
  const std::vector<VectorType>& m_data;
  using PointedToType = typename VectorType::element_type;

public:
  explicit VectorOfPointersConstIterable(const std::vector<VectorType>& data)
    : m_data(data)
  {
    static_assert(elke::is_smart_pointer_v<VectorType>,
                  "VectorType must be a std::unique_ptr or std::shared_ptr.");
  }

  /**A generic iterator for any std::vector with template type `VectorType`.*/
  class VectorOfPointersConstIterator
  {
    /// The vector of data on which this iterator operates.
    const std::vector<VectorType>& m_data;
    /// The current index.
    size_t m_index;

  public:
    /// Constructor.
    VectorOfPointersConstIterator(const std::vector<VectorType>& data,
                                  const size_t index)
      : m_data(data), m_index(index)
    {
    }

    /// De-reference operator.
    const PointedToType& operator*() const { return *m_data[m_index]; }

    // clang-format off
    /// Prefixed increment operator.
    VectorOfPointersConstIterator operator++() { const VectorOfPointersConstIterator i = *this; m_index++; return i; }
    /// Post-fixed increment operator.
    VectorOfPointersConstIterator operator++(int) { m_index++; return *this; }

    /// Boolean equity operator
    bool operator==(const VectorOfPointersConstIterator& rhs) const { return m_index == rhs.m_index; }
    /// Boolean inequity operator
    bool operator!=(const VectorOfPointersConstIterator& rhs) const { return m_index != rhs.m_index; }
    // clang-format on
  };

protected:
  std::vector<VectorType>& data() { return m_data; }
  const std::vector<VectorType>& data() const { return m_data; }

public:
  // clang-format off
  VectorOfPointersConstIterator begin() const { return {m_data, 0}; }
  VectorOfPointersConstIterator end() const { return {m_data, m_data.size()}; }
  size_t size() const { return m_data.size(); }
  // clang-format on
};

} // namespace elke

#endif // ELK_E_GENERIC_ITERATOR_H
