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
std::string VaryingAbstractData::stringValue() const
{
  elkLogicalError("Method not implemented");
}
bool VaryingAbstractData::boolValue() const
{
  elkLogicalError("Method not implemented");
}
int64_t VaryingAbstractData::integerValue() const
{
  elkLogicalError("Method not implemented");
}
double VaryingAbstractData::floatValue() const
{
  elkLogicalError("Method not implemented");
}
std::vector<std::byte> VaryingAbstractData::bytesValue() const
{
  elkLogicalError("Method not implemented");
}

//=============================================== VaryingByteArray
template <>
std::string VaryingTemplatedData<std::vector<std::byte>>::stringValue() const
{
  elkLogicalError("Method not implemented");
}
template <>
bool VaryingTemplatedData<std::vector<std::byte>>::boolValue() const
{
  elkLogicalError("Method not implemented");
}
template <>
int64_t VaryingTemplatedData<std::vector<std::byte>>::integerValue() const
{
  elkLogicalError("Method not implemented");
}
template <>
double VaryingTemplatedData<std::vector<std::byte>>::floatValue() const
{
  elkLogicalError("Method not implemented");
}

//=============================================== VaryingString
template <>
std::string VaryingTemplatedData<std::string>::stringValue() const
{
  return m_value;
}
template <>
bool VaryingTemplatedData<std::string>::boolValue() const
{
  elkLogicalError("Method not implemented");
}
template <>
int64_t VaryingTemplatedData<std::string>::integerValue() const
{
  elkLogicalError("Method not implemented");
}
template <>
double VaryingTemplatedData<std::string>::floatValue() const
{
  elkLogicalError("Method not implemented");
}

//=============================================== VaryingBool
template <>
std::string VaryingTemplatedData<bool>::stringValue() const
{
  elkLogicalError("Method not implemented");
}
template <>
bool VaryingTemplatedData<bool>::boolValue() const
{
  return m_value;
}
template <>
int64_t VaryingTemplatedData<bool>::integerValue() const
{
  elkLogicalError("Method not implemented");
}
template <>
double VaryingTemplatedData<bool>::floatValue() const
{
  elkLogicalError("Method not implemented");
}

//=============================================== VaryingInteger
template <>
std::string VaryingTemplatedData<int64_t>::stringValue() const
{
  elkLogicalError("Method not implemented");
}
template <>
bool VaryingTemplatedData<int64_t>::boolValue() const
{
  elkLogicalError("Method not implemented");
}
template <>
int64_t VaryingTemplatedData<int64_t>::integerValue() const
{
  return m_value;
}
template <>
double VaryingTemplatedData<int64_t>::floatValue() const
{
  elkLogicalError("Method not implemented");
}

//=============================================== VaryingFloat
template <>
std::string VaryingTemplatedData<double>::stringValue() const
{
  elkLogicalError("Method not implemented");
}
template <>
bool VaryingTemplatedData<double>::boolValue() const
{
  elkLogicalError("Method not implemented");
}
template <>
int64_t VaryingTemplatedData<double>::integerValue() const
{
  elkLogicalError("Method not implemented");
}
template <>
double VaryingTemplatedData<double>::floatValue() const
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
VaryingAbstractData::cloneData(
  const std::unique_ptr<VaryingAbstractData>& that)
{
  if (that == nullptr) return nullptr;

  return that->clone();
}


} // namespace elke