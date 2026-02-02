#ifndef ELKE_CORE_UTILITIES_GENERAL_UTILS_H
#define ELKE_CORE_UTILITIES_GENERAL_UTILS_H

#include <string>
#include <vector>

namespace elke
{

struct WarningsAndErrorsData
{
  std::vector<std::string> m_warnings;
  std::vector<std::string> m_errors;
};

struct StatusStrings
{
  std::string m_errors;
  std::string m_warnings;

  void operator+=(const StatusStrings& other)
  {
    m_errors += other.m_errors;
    m_warnings += other.m_warnings;
  }
};

void Abort(const std::string& program_phase);

/**Makes a deep copy of a vector of unique pointers. Requires that the
 *underlying type implements an override for the clone() method.*/
template <typename T>
std::vector<std::unique_ptr<T>>
copyVectorOfUniquePointers(const std::vector<std::unique_ptr<T>>& input)
{
  std::vector<std::unique_ptr<T>> output;

  for (const auto& input_ptr : input)
    output.push_back(std::move(input_ptr->clone()));

  return output;
}

/**Converts any trivially copyable data-type to a vector of bytes.*/
template <typename T>
std::vector<std::byte> toBytes(const T& obj)
{
  // Ensure the type can be safely copied byte-by-byte
  static_assert(
    std::is_trivially_copyable_v<T>,
    "Can only convert trivially copyable objects to a raw byte vector.");

  std::vector<std::byte> bytes(sizeof(T));
  // Copy the object's memory representation into the vector
  std::memcpy(bytes.data(), &obj, sizeof(T));
  return bytes;
}

/**Converts any trivially copyable data-type from a vector of bytes.*/
template <typename T>
T from_bytes(const std::vector<std::byte>& bytes)
{
  // Ensure the type can be safely copied byte-by-byte
  static_assert(
    std::is_trivially_copyable_v<T>,
    "Can only convert trivially copyable objects from a raw byte vector.");

  // Ensure the vector has enough bytes to fill the target data type
  if (bytes.size() < sizeof(T))
    throw std::runtime_error("Byte vector is too small for the target type.");

  T value;
  // Copy the bytes back to the target type's memory location
  std::memcpy(&value, bytes.data(), sizeof(T));
  return value;
}

} // namespace elke

#endif // ELKE_CORE_UTILITIES_GENERAL_UTILS_H
