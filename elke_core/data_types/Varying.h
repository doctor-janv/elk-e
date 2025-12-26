#ifndef ELK_DATA_TYPES_VARYING_H
#define ELK_DATA_TYPES_VARYING_H

#include "VaryingHelpers.h"

#include <iostream>

#include <memory>

namespace elke
{

class Varying
{
  /**type specification*/
  VaryingDataType m_type = VaryingDataType::VOID;
  std::unique_ptr<VaryingAbstractData> m_data = nullptr;

public:
  // Constructors
  /**Default constructor.*/
  Varying() = default;
  /**Generalized constructor for bool, integral- and float-types. This
   * constructor has been specialized for std::string and
   * std::vector<std::byte>.*/
  template <typename T>
  explicit Varying(const T& value)
  {
    constexpr bool is_supported_type =
      IsBool<T>::value or IsFloat<T>::value or IsInteger<T>::value;
    static_assert(is_supported_type,
                  "Constructor called with unsupported type");

    if (IsBool<T>::value) { m_type = VaryingDataType::BOOL; }
    else if (IsFloat<T>::value) { m_type = VaryingDataType::FLOAT; }
    else if (IsInteger<T>::value) { m_type = VaryingDataType::INTEGER; }

    m_data = makeAbstractData(CastValue(value));
  }

  /**Constructor for an arbitrary sequence of bytes value.*/
  explicit Varying(const std::vector<std::byte>& value);
  /**Constructor for a string value.*/
  explicit Varying(const std::string& value);
  /**Constructor for a string literal value.*/
  explicit Varying(const char* value)
    : Varying(not value ? std::string() : std::string(value))
  {
  }

  /**Copy constructor.*/
  Varying(const Varying& other);

  /**Move constructor.*/
  Varying(Varying&& other) noexcept;

  // Copy assignment operator
  /**Assignment operator. i.e., type_A = type_B*/
  Varying& operator=(const Varying& other);

  // Assignment operators
  /**Assigns an arbitrary sequence of bytes value.*/
  Varying& operator=(const std::vector<std::byte>& value);
  /**Assigns a string value.*/
  Varying& operator=(const std::string& value);

  /**Assigns a bool value.*/
  template <typename T, std::enable_if_t<IsBool<T>::value, bool> = true>
  Varying& operator=(const T& value)
  {
    m_type = VaryingDataType::BOOL;
    m_data = std::make_unique<VaryingTemplatedData<bool>>(value);

    return *this;
  }

  /**Assigns an integer value.*/
  template <typename T, std::enable_if_t<IsInteger<T>::value, bool> = true>
  Varying& operator=(const T& value)
  {
    m_type = VaryingDataType::INTEGER;
    m_data = std::make_unique<VaryingTemplatedData<int64_t>>(value);

    return *this;
  }

  /**Assign a floating point value.*/
  template <typename T, std::enable_if_t<IsFloat<T>::value, bool> = true>
  Varying& operator=(const T& value)
  {
    m_type = VaryingDataType::FLOAT;
    m_data = std::make_unique<VaryingTemplatedData<double>>(value);

    return *this;
  }

  /**Equality operator*/
  bool operator==(const Varying& that) const { return *m_data == *that.m_data; }

  /**Inequality operator*/
  bool operator!=(const Varying& that) const { return not(*this == that); }

  /**Relation operators*/
  bool operator>(const Varying& that) const { return *m_data > *that.m_data; }
  /**Relation operators*/
  bool operator>=(const Varying& that) const
  {
    return *this > that or * this == that;
  }
  /**Relation operators*/
  bool operator<(const Varying& that) const { return *m_data < *that.m_data; }
  /**Relation operators*/
  bool operator<=(const Varying& that) const
  {
    return *this < that or * this == that;
  }

  /**Returns a default value for the type required.*/
  template <typename T>
  static T defaultValue()
  {
    return {};
  }

  /**Returns values of type bool if able.*/
  template <typename T>
  BoolType<T> getValue() const
  {
    assertTypeMatch(m_type, VaryingDataType::BOOL);

    return m_data->boolValue();
  }

  /**Returns floating point values if able.*/
  template <typename T>
  FloatType<T> getValue() const
  {
    assertTypeMatch(m_type, VaryingDataType::FLOAT);

    const double value = m_data->floatValue();

    return static_cast<T>(value);
  }

  /**Returns a string if able.*/
  template <typename T>
  StringType<T> getValue() const
  {
    assertTypeMatch(m_type, VaryingDataType::STRING);

    return m_data->stringValue();
  }

  /**Returns a signed integer if able.*/
  template <typename T>
  SignedIntegerType<T> getValue() const
  {
    assertTypeMatch(m_type, VaryingDataType::INTEGER);

    const int64_t value = m_data->integerValue();

    return static_cast<T>(value);
  }

  /**Returns an unsigned integer if able.*/
  template <typename T>
  UnsignedIntegerType<T> getValue() const
  {
    assertTypeMatch(m_type, VaryingDataType::INTEGER);

    const int64_t value = m_data->integerValue();

    if (value < 0)
      throw std::logic_error(std::string(__PRETTY_FUNCTION__) +
                             ": Attempt to cast negative number to unsigned.");

    return static_cast<T>(value);
  }

  /**Returns the string value if valid. Otherwise, throws std::logic_error.*/
  std::string stringValue() const;
  /**Returns the bool value if valid. Otherwise, throws std::logic_error.*/
  bool boolValue() const;
  /**Returns the integer value if valid. Otherwise, throws std::logic_error.*/
  int64_t integerValue() const;
  /**Returns the float value if valid. Otherwise, throws std::logic_error.*/
  double floatValue() const;

  /**Returns the current-type of the variable.*/
  VaryingDataType type() const { return m_type; }
  /**Returns the string type name of the type.*/
  std::string typeName() const { return varyingDataTypeStringName(m_type); }

  /**Returns a string value for the value.*/
  std::string convertToString(bool with_type = true) const;

  /**Determines if the value held is convertible to the target type.*/
  bool isConvertibleToType(VaryingDataType target_type) const;

  ~Varying() = default;

private:
  /**Checks if two VaryingDataType values match.
   * type A is matched against type B.*/
  void assertTypeMatch(VaryingDataType type_A,
                       VaryingDataType type_B_required) const;
}; // class Varying

} // namespace elke

/**Stream operator*/
std::ostream& operator<<(std::ostream& outstr, const elke::Varying& value);

#endif // ELK_DATA_TYPES_VARYING_H
