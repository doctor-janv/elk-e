#ifndef ELK_E_SCALARDATA_H
#define ELK_E_SCALARDATA_H

#include "ScalarData.h"

#include "elke_core/data_types/VaryingHelpers.h"

#include <any>

namespace elke
{

enum class ScalarType : int
{
  VOID = 0,            ///< Basically undefined or null
  ARBITRARY_BYTES = 1, ///< Basic sequence of bytes
  STRING = 2,          ///< Datatype mapping to std::string
  BOOL = 3,            ///< Datatype mapping to bool
  INTEGER = 4,         ///< Datatype mapping to int64_t
  FLOAT = 5            ///< Datatype mapping to double
};

/**Templated structure to statically determine a type enum.*/
template <typename T>
struct ScalarTypeEvaluation
{
  // clang-format off
  constexpr static ScalarType type =
    IsString<T>::value ? ScalarType::STRING :
    IsBool<T>::value ? ScalarType::BOOL :
    IsInteger<T>::value ? ScalarType::INTEGER :
    IsFloat<T>::value ? ScalarType::FLOAT :
    ScalarType::VOID;
  // clang-format on
};

// ###################################################################
/**Implementation of arbitrary scalar data.*/
class ScalarData
{
  /**The overall scalar type.*/
  ScalarType m_scalar_type;
  /**The data value.*/
  std::any m_value;

public:
  /**No value constructor.*/
  ScalarData() : m_scalar_type(ScalarType::VOID) {}

  /**No value constructor with type specified via template.*/
  template <typename T>
  ScalarData() : m_scalar_type(ScalarTypeEvaluation<T>::type), m_value(T())
  {
  }

  /**Constructor for value initialization.*/
  template <typename T>
  explicit ScalarData(const T& value)
    : m_scalar_type(ScalarTypeEvaluation<T>::type), m_value(value)
  {
  }

  /**Constructor for specialized string literals.*/
  explicit ScalarData(const char* string_value)
    : m_scalar_type(ScalarType::STRING), m_value(std::string(string_value))
  {
  }

  /**Returns a const reference to the scalar value.*/
  template <typename T>
  const T& getValue() const
  {
    return std::any_cast<T&>(m_value);
  }

  /**Returns a non-const reference to the scalar value.*/
  template <typename T>
  T& getValue()
  {
    return std::any_cast<T&>(m_value);
  }
};

} // namespace elke

#endif // ELK_E_SCALARDATA_H
