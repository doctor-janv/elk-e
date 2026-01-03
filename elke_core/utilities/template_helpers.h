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
} // namespace elke
#endif // ELK_E_TEMPLATE_HELPERS_H
