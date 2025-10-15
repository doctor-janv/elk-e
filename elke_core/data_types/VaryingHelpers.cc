#include "VaryingHelpers.h"

#include "elke_core/output/elk_exceptions.h"

#include <algorithm>
#include <sstream>

namespace elke
{

/**Provides a string-name for an enumerated VaryingDataType.*/
std::string VaryingDataTypeStringName(const VaryingDataType type)
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
std::string VaryingAbstractData::StringValue() const
{
  elkLogicalError("Method not implemented");
}
bool VaryingAbstractData::BoolValue() const
{
  elkLogicalError("Method not implemented");
}
int64_t VaryingAbstractData::IntegerValue() const
{
  elkLogicalError("Method not implemented");
}
double VaryingAbstractData::FloatValue() const
{
  elkLogicalError("Method not implemented");
}
std::vector<std::byte> VaryingAbstractData::BytesValue() const
{
  elkLogicalError("Method not implemented");
}

//=============================================== VaryingByteArray
template <>
std::string VaryingTemplatedData<std::vector<std::byte>>::StringValue() const
{
  elkLogicalError("Method not implemented");
}
template <>
bool VaryingTemplatedData<std::vector<std::byte>>::BoolValue() const
{
  elkLogicalError("Method not implemented");
}
template <>
int64_t VaryingTemplatedData<std::vector<std::byte>>::IntegerValue() const
{
  elkLogicalError("Method not implemented");
}
template <>
double VaryingTemplatedData<std::vector<std::byte>>::FloatValue() const
{
  elkLogicalError("Method not implemented");
}

//=============================================== VaryingString
template <>
std::string VaryingTemplatedData<std::string>::StringValue() const
{
  return value_;
}
template <>
bool VaryingTemplatedData<std::string>::BoolValue() const
{
  elkLogicalError("Method not implemented");
}
template <>
int64_t VaryingTemplatedData<std::string>::IntegerValue() const
{
  elkLogicalError("Method not implemented");
}
template <>
double VaryingTemplatedData<std::string>::FloatValue() const
{
  elkLogicalError("Method not implemented");
}

//=============================================== VaryingBool
template <>
std::string VaryingTemplatedData<bool>::StringValue() const
{
  elkLogicalError("Method not implemented");
}
template <>
bool VaryingTemplatedData<bool>::BoolValue() const
{
  return value_;
}
template <>
int64_t VaryingTemplatedData<bool>::IntegerValue() const
{
  elkLogicalError("Method not implemented");
}
template <>
double VaryingTemplatedData<bool>::FloatValue() const
{
  elkLogicalError("Method not implemented");
}

//=============================================== VaryingInteger
template <>
std::string VaryingTemplatedData<int64_t>::StringValue() const
{
  elkLogicalError("Method not implemented");
}
template <>
bool VaryingTemplatedData<int64_t>::BoolValue() const
{
  elkLogicalError("Method not implemented");
}
template <>
int64_t VaryingTemplatedData<int64_t>::IntegerValue() const
{
  return value_;
}
template <>
double VaryingTemplatedData<int64_t>::FloatValue() const
{
  elkLogicalError("Method not implemented");
}

//=============================================== VaryingFloat
template <>
std::string VaryingTemplatedData<double>::StringValue() const
{
  elkLogicalError("Method not implemented");
}
template <>
bool VaryingTemplatedData<double>::BoolValue() const
{
  elkLogicalError("Method not implemented");
}
template <>
int64_t VaryingTemplatedData<double>::IntegerValue() const
{
  elkLogicalError("Method not implemented");
}
template <>
double VaryingTemplatedData<double>::FloatValue() const
{
  return value_;
}

std::unique_ptr<VaryingAbstractData> MakeAbstractData(const bool& value)
{
  return std::make_unique<VaryingTemplatedData<bool>>(value);
}

std::unique_ptr<VaryingAbstractData> MakeAbstractData(const int64_t& value)
{
  return std::make_unique<VaryingTemplatedData<int64_t>>(value);
}

std::unique_ptr<VaryingAbstractData> MakeAbstractData(const double& value)
{
  return std::make_unique<VaryingTemplatedData<double>>(value);
}

} // namespace elke