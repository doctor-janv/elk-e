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
using BoolType = std::enable_if_t<IsBool<T>::value, T>;
template <typename T>
using FloatType = std::enable_if_t<IsFloat<T>::value, T>;
template <typename T>
using IntegerType = std::enable_if_t<IsInteger<T>::value, T>;

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
public:
  virtual std::string StringValue() const;
  virtual bool BoolValue() const;
  virtual int64_t IntegerValue() const;
  virtual double FloatValue() const;
  virtual std::vector<std::byte> BytesValue() const;

  virtual std::unique_ptr<VaryingAbstractData> Clone() const = 0;
  virtual size_t Size() const = 0;

  virtual bool operator==(const VaryingAbstractData& that) const = 0;
  virtual bool operator!=(const VaryingAbstractData& that) const = 0;
  virtual bool operator>(const VaryingAbstractData& that) const = 0;
  virtual bool operator<(const VaryingAbstractData& that) const = 0;
  virtual bool operator>=(const VaryingAbstractData& that) const = 0;
  virtual bool operator<=(const VaryingAbstractData& that) const = 0;

  VaryingDataType Type() const { return type_; }

  virtual ~VaryingAbstractData() = default;

protected:
  VaryingDataType type_;
  explicit VaryingAbstractData(const VaryingDataType type) : type_(type) {}
};

/**Templated arbitrary types.*/
template <typename T>
class VaryingTemplatedData final : public VaryingAbstractData
{
public:
  // clang-format off
    explicit VaryingTemplatedData(T value)
      : VaryingAbstractData(IsByteArray<T>::value ? VaryingDataType::ARBITRARY_BYTES :
                    IsString<T>::value ? VaryingDataType::STRING :
                    IsBool<T>::value ? VaryingDataType::BOOL :
                    IsInteger<T>::value ? VaryingDataType::INTEGER :
                    IsFloat<T>::value ? VaryingDataType::FLOAT :
                    VaryingDataType::VOID),
      value_(value)
    {
    }
  // clang-format on
  std::string StringValue() const override;
  bool BoolValue() const override;
  int64_t IntegerValue() const override;
  double FloatValue() const override;

  std::unique_ptr<VaryingAbstractData> Clone() const override
  {
    return std::make_unique<VaryingTemplatedData>(value_);
  }
  size_t Size() const override { return sizeof(T); }

  bool operator==(const VaryingAbstractData& that) const override
  {
    if (type_ != that.Type()) return false;

    switch (this->Type())
    {
      case VaryingDataType::ARBITRARY_BYTES:
        return BytesValue() == that.BytesValue();
      case VaryingDataType::STRING:
        return StringValue() == that.StringValue();
      case VaryingDataType::BOOL:
        return BoolValue() == that.BoolValue();
      case VaryingDataType::INTEGER:
        return IntegerValue() == that.IntegerValue();
      case VaryingDataType::FLOAT:
        return FloatValue() == that.FloatValue();
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
    if (type_ != that.Type()) return false;

    switch (this->Type())
    {
      case VaryingDataType::ARBITRARY_BYTES:
        return BytesValue() > that.BytesValue();
      case VaryingDataType::STRING:
        return StringValue() > that.StringValue();
      case VaryingDataType::BOOL:
        return BoolValue() > that.BoolValue();
      case VaryingDataType::INTEGER:
        return IntegerValue() > that.IntegerValue();
      case VaryingDataType::FLOAT:
        return FloatValue() > that.FloatValue();
      case VaryingDataType::VOID:
      default:
        return false;
    }
  }
  bool operator<(const VaryingAbstractData& that) const override
  {
    if (type_ != that.Type()) return false;

    switch (this->Type())
    {
      case VaryingDataType::ARBITRARY_BYTES:
        return BytesValue() < that.BytesValue();
      case VaryingDataType::STRING:
        return StringValue() < that.StringValue();
      case VaryingDataType::BOOL:
        return BoolValue() < that.BoolValue();
      case VaryingDataType::INTEGER:
        return IntegerValue() < that.IntegerValue();
      case VaryingDataType::FLOAT:
        return FloatValue() < that.FloatValue();
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

private:
  T value_;
};

// ######################################################### Helper functions
/**Provides a string-name for an enumerated VaryingDataType.*/
std::string VaryingDataTypeStringName(VaryingDataType type);

std::unique_ptr<VaryingAbstractData> MakeAbstractData(const bool& value);

std::unique_ptr<VaryingAbstractData> MakeAbstractData(const int64_t& value);

std::unique_ptr<VaryingAbstractData> MakeAbstractData(const double& value);

} // namespace elke

#endif // ELK_E_VARYINGHELPERS_H
