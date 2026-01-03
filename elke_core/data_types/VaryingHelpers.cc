#include "VaryingHelpers.h"

#include "elke_core/output/elk_exceptions.h"

#include <algorithm>
#include <sstream>

namespace elke
{

/**Provides a string-name for an enumerated VaryingDataType.*/
std::string varyingDataTypeStringName(const VaryingDataType type)
{
  switch (type)
  {
    case VaryingDataType::VOID:
      return "VOID";
    case VaryingDataType::ARBITRARY_BYTES:
      return "ARBITRARY_BYTES";
    case VaryingDataType::STRING:
      return "STRING";
    case VaryingDataType::BOOL:
      return "BOOL";
    case VaryingDataType::INTEGER:
      return "INTEGER";
    case VaryingDataType::FLOAT:
      return "FLOAT";
    default:
      return "UNKNOWN";
  }
}

//=============================================== VaryingType Base
const std::vector<std::byte>& VaryingAbstractData::bytesValue() const
{
  elkLogicalError("Method not implemented");
}
const std::string& VaryingAbstractData::stringValue() const
{
  elkLogicalError("Method not implemented");
}
const bool& VaryingAbstractData::boolValue() const
{
  elkLogicalError("Method not implemented");
}
const int64_t& VaryingAbstractData::integerValue() const
{
  elkLogicalError("Method not implemented");
}
const double& VaryingAbstractData::floatValue() const
{
  elkLogicalError("Method not implemented");
}

//=============================================== VaryingByteArray
template <>
const std::vector<std::byte>&
VaryingTemplatedData<std::vector<std::byte>>::bytesValue() const
{
  return m_value;
}
template <>
const std::string& VaryingTemplatedData<std::vector<std::byte>>::stringValue() const
{
  elkLogicalError("Method not implemented");
}
template <>
const bool& VaryingTemplatedData<std::vector<std::byte>>::boolValue() const
{
  elkLogicalError("Method not implemented");
}
template <>
const int64_t& VaryingTemplatedData<std::vector<std::byte>>::integerValue() const
{
  elkLogicalError("Method not implemented");
}
template <>
const double& VaryingTemplatedData<std::vector<std::byte>>::floatValue() const
{
  elkLogicalError("Method not implemented");
}

//=============================================== VaryingString
template <>
const std::vector<std::byte>&
VaryingTemplatedData<std::string>::bytesValue() const
{
  elkLogicalError("Method not implemented");
}
template <>
const std::string& VaryingTemplatedData<std::string>::stringValue() const
{
  return m_value;
}
template <>
const bool& VaryingTemplatedData<std::string>::boolValue() const
{
  elkLogicalError("Method not implemented");
}
template <>
const int64_t& VaryingTemplatedData<std::string>::integerValue() const
{
  elkLogicalError("Method not implemented");
}
template <>
const double& VaryingTemplatedData<std::string>::floatValue() const
{
  elkLogicalError("Method not implemented");
}

//=============================================== VaryingBool
template <>
const std::vector<std::byte>&
VaryingTemplatedData<bool>::bytesValue() const
{
  elkLogicalError("Method not implemented");
}
template <>
const std::string& VaryingTemplatedData<bool>::stringValue() const
{
  elkLogicalError("Method not implemented");
}
template <>
const bool& VaryingTemplatedData<bool>::boolValue() const
{
  return m_value;
}
template <>
const int64_t& VaryingTemplatedData<bool>::integerValue() const
{
  elkLogicalError("Method not implemented");
}
template <>
const double& VaryingTemplatedData<bool>::floatValue() const
{
  elkLogicalError("Method not implemented");
}

//=============================================== VaryingInteger
template <>
const std::vector<std::byte>&
VaryingTemplatedData<int64_t>::bytesValue() const
{
  elkLogicalError("Method not implemented");
}
template <>
const std::string& VaryingTemplatedData<int64_t>::stringValue() const
{
  elkLogicalError("Method not implemented");
}
template <>
const bool& VaryingTemplatedData<int64_t>::boolValue() const
{
  elkLogicalError("Method not implemented");
}
template <>
const int64_t& VaryingTemplatedData<int64_t>::integerValue() const
{
  return m_value;
}
template <>
const double& VaryingTemplatedData<int64_t>::floatValue() const
{
  elkLogicalError("Method not implemented");
}

//=============================================== VaryingFloat
template <>
const std::vector<std::byte>&
VaryingTemplatedData<double>::bytesValue() const
{
  elkLogicalError("Method not implemented");
}
template <>
const std::string& VaryingTemplatedData<double>::stringValue() const
{
  elkLogicalError("Method not implemented");
}
template <>
const bool& VaryingTemplatedData<double>::boolValue() const
{
  elkLogicalError("Method not implemented");
}
template <>
const int64_t& VaryingTemplatedData<double>::integerValue() const
{
  elkLogicalError("Method not implemented");
}
template <>
const double& VaryingTemplatedData<double>::floatValue() const
{
  return m_value;
}

std::unique_ptr<VaryingAbstractData> makeAbstractData(const bool& value)
{
  return std::make_unique<VaryingTemplatedData<bool>>(value);
}

std::unique_ptr<VaryingAbstractData> makeAbstractData(const int64_t& value)
{
  return std::make_unique<VaryingTemplatedData<int64_t>>(value);
}

std::unique_ptr<VaryingAbstractData> makeAbstractData(const double& value)
{
  return std::make_unique<VaryingTemplatedData<double>>(value);
}

std::unique_ptr<VaryingAbstractData>
VaryingAbstractData::cloneData(const std::unique_ptr<VaryingAbstractData>& that)
{
  if (that == nullptr) return nullptr;

  return that->clone();
}

} // namespace elke