#ifndef ELK_E_TEMPLATE_HELPERS_H
#define ELK_E_TEMPLATE_HELPERS_H

#include <memory>
#include <type_traits>

namespace elke
{
// Primary template (For raw pointers)
template <typename T>
struct is_smart_pointer : std::false_type {};

// Specialization for std::shared_ptr
template <typename T>
struct is_smart_pointer<std::shared_ptr<T>> : std::true_type {};

// Specialization for std::unique_ptr
template <typename T>
struct is_smart_pointer<std::unique_ptr<T>> : std::true_type {};

// Helper constant for convenience
template <typename T>
constexpr bool is_smart_pointer_v = is_smart_pointer<T>::value;

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

template <typename T> //NOLINT
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

template <typename T> //NOLINT
struct IsVectorOfScalars
{
  static constexpr bool value = false;
};

template <typename T, typename A>
struct IsVectorOfScalars<std::vector<T, A>>
{
  static constexpr bool value = IsScalar<T>::value;;
};


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

/**A generic helper class to make any derived class cloneable.*/
template <typename Derived>
class Clonable
{
public:
  /** This function can only be called on an actual Derived object,
   * as it is implemented in the template base.*/
  Derived* clone_helper() const
  {
    // Use static_cast to convert 'this' pointer (to Clonable<Derived>)
    // to a 'Derived*' pointer, then dereference to call the copy constructor.
    return new Derived(static_cast<const Derived&>(*this));
  }

  /**Virtual destructor is still needed for proper cleanup via base pointer*/
  virtual ~Clonable() = default;

protected:
  // Protected constructors to prevent direct instantiation of Clonable
  Clonable() = default;
  Clonable(const Clonable&) = default;
  Clonable& operator=(const Clonable&) = default;
  Clonable(Clonable&&) = default;
  Clonable& operator=(Clonable&&) = default;
};

} // namespace elke
#endif // ELK_E_TEMPLATE_HELPERS_H
