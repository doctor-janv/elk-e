#ifndef ELK_E_SCALARDATA_H
#define ELK_E_SCALARDATA_H

#include "ScalarValue.h"

#include "elke_core/utilities/template_helpers.h"

#include <any>

namespace elke
{

enum class ScalarType : int
{
  VOID = 0,    ///< Basically undefined or null
  STRING = 1,  ///< Datatype mapping to std::string
  BOOL = 2,    ///< Datatype mapping to bool
  INTEGER = 3, ///< Datatype mapping to any integer type
  FLOAT = 4    ///< Datatype mapping to any floating point type
};

/**Returns a string representation of ScalarType.
 * \param type The type to convert to a string.
 */
std::string scalarTypeStringName(ScalarType type);

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
/**Class with templated constructors to build scalar-type values of several
 *primitive types. Mainly a wrapper around `std::any` this class provides
 * some flexibility that `std::any` does not have. The types of the value stored
 * is restricted to `elke::ScalarType` which allows.:
 * - `ScalarType::VOID`
 * - `ScalarType::STRING`, generically mapped to `std::string`
 * - `ScalarType::BOOL`, generically mapped to `bool`
 * - `ScalarType::INTEGER`, generically mapped to `int64_t` (`long`)
 * - `ScalarType::FLOAT`, generically mapped `double`
 *
 * Example uses:\n
 * ```c++
const auto v0 = ScalarValue();        // Creates a VOID type
const auto v1 = ScalarValue(std::string{}); // Creates a STRING type
const auto v2 = ScalarValue(int{});  // Creates an INTEGER type
const auto v3 = ScalarValue(float{}); // Creates a FLOAT type
 * ```
 * ```c++
const auto v0 = ScalarValue();                     // Creates a VOID type
const auto v1a = ScalarValue("Name");              // Creates a STRING type from
literal const auto v1b = ScalarValue(std::string("Name")); // Creates a STRING
type const auto v2 = ScalarValue(12);                   // Creates an INTEGER
type const auto v3 = ScalarValue(1.2);                  // Creates a FLOAT type
 * ```
 *
 * Noteworthy methods:
 * - `ScalarValue::type` returns the `ScalarType`
 * - `ScalarValue::getValue` returns the templated type (Can throw an
exception).
 * - `ScalarType::convertToString` provides a string representation of the data.
 */
class ScalarValue
{
  /**The overall scalar type.*/
  ScalarType m_scalar_type;
  /**The data value.*/
  std::any m_value;

public:
  /**No value constructor.*/
  ScalarValue() : m_scalar_type(ScalarType::VOID) {}

  /**Copy constructor.*/
  ScalarValue(const ScalarValue&) = default;

  //======================= Value provided
  /**Constructor for value initialization.*/
  template <typename T, std::enable_if_t<IsString<T>::value, bool> = true>
  explicit ScalarValue(const T& value)
    : m_scalar_type(ScalarTypeEvaluation<T>::type), m_value(std::string{value})
  {
  }
  /**Constructor for value initialization.*/
  template <typename T, std::enable_if_t<IsBool<T>::value, bool> = true>
  explicit ScalarValue(const T& value)
    : m_scalar_type(ScalarTypeEvaluation<T>::type), m_value(bool{value})
  {
  }
  /**Constructor for value initialization.*/
  template <typename T, std::enable_if_t<IsInteger<T>::value, bool> = true>
  explicit ScalarValue(const T& value)
    : m_scalar_type(ScalarTypeEvaluation<T>::type), m_value(int64_t{value})
  {
  }
  /**Constructor for value initialization.*/
  template <typename T, std::enable_if_t<IsFloat<T>::value, bool> = true>
  explicit ScalarValue(const T& value)
    : m_scalar_type(ScalarTypeEvaluation<T>::type), m_value(double{value})
  {
  }

  /**Constructor for specialized string literals.
   * \param string_value A string-literal type argument.
   */
  explicit ScalarValue(const char* string_value)
    : m_scalar_type(ScalarType::STRING), m_value(std::string(string_value))
  {
  }

  /**Returns the scalar type.*/
  ScalarType type() const { return m_scalar_type; }

  /**Returns the internal implementation type as a string.*/
  std::string internalTypeString() const {
    return m_value.type().name();
  }

  /**Returns a string representation of the type.*/
  std::string typeString() const { return scalarTypeStringName(m_scalar_type); }

  /**Determines if the value held is convertible to the target type.
   * \param target_type The type to which the conversion is intended.
   */
  bool isConvertibleToType(ScalarType target_type) const;

  /**Returns a new scalar value of this value converted to the target type.*/
  ScalarValue convertedToType(ScalarType target_type) const;

  /**Returns a const reference to the scalar value. Can throw
   * std::bad_any_cast.*/
  template <typename T>
  T getValue() const
  {
    return std::any_cast<T>(m_value);
  }

  /**Returns a non-const reference to the scalar value. Can throw
   * std::bad_any_cast.*/
  template <typename T>
  T getValue()
  {
    return std::any_cast<T>(m_value);
  }

  /**Converts the underlying value to a string using a stream operator.
   * \param with_type If true, will append the type to the back of the string,
   *                  for example "12" becomes "12(INTEGER)".
   */
  std::string convertToString(bool with_type = false) const;
};

} // namespace elke

/**Stream operator.*/
std::ostream& operator<<(std::ostream& outstr, const elke::ScalarValue& value);

#endif // ELK_E_SCALARDATA_H
