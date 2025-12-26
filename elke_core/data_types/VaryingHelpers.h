#ifndef ELK_E_VARYINGHELPERS_H
#define ELK_E_VARYINGHELPERS_H

#include <vector>

namespace elke
{

/**Enumeration of data-types supported by Varying*/
enum class VaryingDataType : int
{
  VOID = 0,            ///< Basically undefined or null
  ARBITRARY_BYTES = 1, ///< Basic sequence of bytes
  STRING = 2,          ///< Datatype mapping to std::string
  BOOL = 3,            ///< Datatype mapping to bool
  INTEGER = 4,         ///< Datatype mapping to int64_t
  FLOAT = 5            ///< Datatype mapping to double
};

// ######################################################### Template helpers
template <typename T>
struct IsByteArray
{
  static constexpr bool value = std::is_same_v<T, std::vector<std::byte>>;
};
template <typename T>
struct IsBool
{
  static constexpr bool value = std::is_same_v<T, bool>;
};
template <typename T>
struct IsString
{
  static constexpr bool value =
    std::is_same_v<T, std::string> or std::is_same_v<T, char*>;
};
template <typename T>
struct IsFloat
{
  static constexpr bool value = std::is_floating_point_v<T>;
};
template <typename T>
struct IsInteger
{
  static constexpr bool value =
    std::is_integral_v<T> and not std::is_same_v<T, bool>;
};

template <typename T>
struct IsScalar
{
  static constexpr bool value = IsBool<T>::value or IsString<T>::value or
                                IsInteger<T>::value or IsFloat<T>::value;
};

template <typename T>
struct IsVector
{
  static constexpr bool value = false;
};

// Partial specialization for any std::vector
// T is the element type, A is the allocator type (which has a default value)
template <typename T, typename A>
struct IsVector<std::vector<T, A>>
{
  static constexpr bool value = true;
};

template <typename T>
struct IsVectorOfScalars
{
  static constexpr bool value = false;
};

template <typename T, typename A>
struct IsVectorOfScalars<std::vector<T, A>>
{
  static constexpr bool value = IsScalar<T>::value;;
};

template <typename T>
using BoolType = std::enable_if_t<IsBool<T>::value, T>;
template <typename T>
using FloatType = std::enable_if_t<IsFloat<T>::value, T>;
template <typename T>
using IntegerType = std::enable_if_t<IsInteger<T>::value, T>;
template <typename T>
using ScalarType = std::enable_if_t<IsScalar<T>::value, T>;
template <typename T>
using VectorType = std::enable_if_t<IsVector<T>::value, T>;

template <typename T>
using BoolStorageType = std::enable_if_t<IsBool<T>::value, bool>;
template <typename T>
using FloatStorageType = std::enable_if_t<IsFloat<T>::value, double>;
template <typename T>
using IntegerStorageType = std::enable_if_t<IsInteger<T>::value, int64_t>;

template <typename T>
BoolStorageType<T> CastValue(const T& value)
{
  return value;
}

template <typename T>
FloatStorageType<T> CastValue(const T& value)
{
  return static_cast<double>(value);
}

template <typename T>
IntegerStorageType<T> CastValue(const T& value)
{
  return static_cast<int64_t>(value);
}

// More Helpers
template <typename T>
struct IsSignedInteger
{
  static constexpr bool value = std::is_integral_v<T> and
                                std::is_signed_v<T> and
                                not std::is_same_v<T, bool>;
};
template <typename T>
struct IsUnsignedInteger
{
  static constexpr bool value = std::is_integral_v<T> and
                                std::is_unsigned_v<T> and
                                not std::is_same_v<T, bool>;
};

template <typename T>
using StringType = std::enable_if_t<IsString<T>::value, T>;
template <typename T>
using SignedIntegerType = std::enable_if_t<IsSignedInteger<T>::value, T>;
template <typename T>
using UnsignedIntegerType = std::enable_if_t<IsUnsignedInteger<T>::value, T>;

// ######################################################### Helper classes
/**This acts as a base class for templated child arbitrary types.*/
class VaryingAbstractData
{
protected:
  VaryingDataType m_type;

public:
  virtual std::string stringValue() const;
  virtual bool boolValue() const;
  virtual int64_t integerValue() const;
  virtual double floatValue() const;
  virtual std::vector<std::byte> bytesValue() const;

  virtual bool operator==(const VaryingAbstractData& that) const = 0;
  virtual bool operator!=(const VaryingAbstractData& that) const = 0;
  virtual bool operator>(const VaryingAbstractData& that) const = 0;
  virtual bool operator<(const VaryingAbstractData& that) const = 0;
  virtual bool operator>=(const VaryingAbstractData& that) const = 0;
  virtual bool operator<=(const VaryingAbstractData& that) const = 0;

  VaryingDataType type() const { return m_type; }

  /**Makes a clone of a unique_ptr to VaryingAbstractData.*/
  static std::unique_ptr<VaryingAbstractData>
  cloneData(const std::unique_ptr<VaryingAbstractData>& that);

  virtual ~VaryingAbstractData() = default;

protected:
  explicit VaryingAbstractData(const VaryingDataType type) : m_type(type) {}
  virtual std::unique_ptr<VaryingAbstractData> clone() const = 0;
};

// ###################################################################
/**Templated arbitrary types.*/
template <typename T>
class VaryingTemplatedData final : public VaryingAbstractData
{
  T m_value;

public:
  // clang-format off
    explicit VaryingTemplatedData(T value)
      : VaryingAbstractData(IsByteArray<T>::value ? VaryingDataType::ARBITRARY_BYTES :
                    IsString<T>::value ? VaryingDataType::STRING :
                    IsBool<T>::value ? VaryingDataType::BOOL :
                    IsInteger<T>::value ? VaryingDataType::INTEGER :
                    IsFloat<T>::value ? VaryingDataType::FLOAT :
                    VaryingDataType::VOID),
      m_value(value)
    {
    }
  // clang-format on
  std::string stringValue() const override;
  bool boolValue() const override;
  int64_t integerValue() const override;
  double floatValue() const override;

  bool operator==(const VaryingAbstractData& that) const override
  {
    if (m_type != that.type()) return false;

    switch (this->type())
    {
      case VaryingDataType::ARBITRARY_BYTES:
        return bytesValue() == that.bytesValue();
      case VaryingDataType::STRING:
        return stringValue() == that.stringValue();
      case VaryingDataType::BOOL:
        return boolValue() == that.boolValue();
      case VaryingDataType::INTEGER:
        return integerValue() == that.integerValue();
      case VaryingDataType::FLOAT:
        return floatValue() == that.floatValue();
      case VaryingDataType::VOID:
      default:
        return false;
    }
  }
  bool operator!=(const VaryingAbstractData& that) const override
  {
    return not(*this == that);
  }
  bool operator>(const VaryingAbstractData& that) const override
  {
    if (m_type != that.type()) return false;

    switch (this->type())
    {
      case VaryingDataType::ARBITRARY_BYTES:
        return bytesValue() > that.bytesValue();
      case VaryingDataType::STRING:
        return stringValue() > that.stringValue();
      case VaryingDataType::BOOL:
        return boolValue() > that.boolValue();
      case VaryingDataType::INTEGER:
        return integerValue() > that.integerValue();
      case VaryingDataType::FLOAT:
        return floatValue() > that.floatValue();
      case VaryingDataType::VOID:
      default:
        return false;
    }
  }
  bool operator<(const VaryingAbstractData& that) const override
  {
    if (m_type != that.type()) return false;

    switch (this->type())
    {
      case VaryingDataType::ARBITRARY_BYTES:
        return bytesValue() < that.bytesValue();
      case VaryingDataType::STRING:
        return stringValue() < that.stringValue();
      case VaryingDataType::BOOL:
        return boolValue() < that.boolValue();
      case VaryingDataType::INTEGER:
        return integerValue() < that.integerValue();
      case VaryingDataType::FLOAT:
        return floatValue() < that.floatValue();
      case VaryingDataType::VOID:
      default:
        return false;
    }
  }
  bool operator>=(const VaryingAbstractData& that) const override
  {
    return *this > that or * this == that;
  }
  bool operator<=(const VaryingAbstractData& that) const override
  {
    return *this < that or * this == that;
  }

protected:
  std::unique_ptr<VaryingAbstractData> clone() const override
  {
    return std::make_unique<VaryingTemplatedData>(m_value);
  }
};

// ######################################################### Helper functions
/**Provides a string-name for an enumerated VaryingDataType.*/
std::string varyingDataTypeStringName(VaryingDataType type);

std::unique_ptr<VaryingAbstractData> makeAbstractData(const bool& value);

std::unique_ptr<VaryingAbstractData> makeAbstractData(const int64_t& value);

std::unique_ptr<VaryingAbstractData> makeAbstractData(const double& value);

} // namespace elke

#endif // ELK_E_VARYINGHELPERS_H
