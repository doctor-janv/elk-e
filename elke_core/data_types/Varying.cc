#include "Varying.h"

#include <sstream>

#include "elke_core/output/elk_exceptions.h"
#include "elke_core/utilities/string_utils.h"

namespace elke
{

/**Checks if two VaryingDataType values match.
 * type A is matched against type B. Throws std::logic_error if not.*/
void Varying::assertTypeMatch(const VaryingDataType type_A,
                              const VaryingDataType type_B_required) const
{
  if (type_A != type_B_required)
    throw std::logic_error("Varying data type " + typeName() +
                           " does not "
                           "correspond to the required type, " +
                           varyingDataTypeStringName(type_B_required));
}

// ###################################################################
// Constructors

/**Constructor for an arbitrary sequence of bytes value.*/
Varying::Varying(const std::vector<std::byte>& value)
  : m_type(VaryingDataType::ARBITRARY_BYTES)
{
  m_data =
    std::make_unique<VaryingTemplatedData<std::vector<std::byte>>>(value);
}

/**Constructor for a string value.*/
Varying::Varying(const std::string& value)
  : m_type(VaryingDataType::STRING),
    m_data(std::make_unique<VaryingTemplatedData<std::string>>(value))
{
}

/**Copy constructor.*/
Varying::Varying(const Varying& other)
{
  m_data = VaryingAbstractData::cloneData(other.m_data);
  m_type = other.m_type;
}

/**Move constructor.*/
Varying::Varying(Varying&& other) noexcept
{
  std::swap(m_data, other.m_data);
  std::swap(m_type, other.m_type);
}

/**Assignment operator. i.e., type_A = type_B*/
Varying& Varying::operator=(const Varying& other)
{
  if (this != &other)
  {
    m_data = VaryingAbstractData::cloneData(other.m_data);
    m_type = other.m_type;
  }
  return *this;
}

// ###################################################################
//  Assignments
/**Assigns an arbitrary sequence of bytes value.*/
Varying& Varying::operator=(const std::vector<std::byte>& value)
{
  m_type = VaryingDataType::ARBITRARY_BYTES;
  m_data =
    std::make_unique<VaryingTemplatedData<std::vector<std::byte>>>(value);
  return *this;
}

/**Assigns a string value.*/
Varying& Varying::operator=(const std::string& value)
{
  m_type = VaryingDataType::STRING;
  m_data = std::make_unique<VaryingTemplatedData<std::string>>(value);
  return *this;
}

// ###################################################################
//  Get values
/**Returns the bytes value if valid. Otherwise, throws std::logic_error.*/
const std::vector<std::byte>& Varying::bytesValue() const
{
  assertTypeMatch(m_type, VaryingDataType::ARBITRARY_BYTES);

  return m_data->bytesValue();
}

/**Returns the string value if valid. Otherwise, throws std::logic_error.*/
const std::string& Varying::stringValue() const
{
  assertTypeMatch(m_type, VaryingDataType::STRING);

  return m_data->stringValue();
}

/**Returns the bool value if valid. Otherwise, throws std::logic_error.*/
const bool& Varying::boolValue() const
{
  assertTypeMatch(m_type, VaryingDataType::BOOL);

  return m_data->boolValue();
}

/**Returns the integer value if valid. Otherwise, throws std::logic_error.*/
const int64_t& Varying::integerValue() const
{
  assertTypeMatch(m_type, VaryingDataType::INTEGER);

  return m_data->integerValue();
}

/**Returns the float value if valid. Otherwise, throws std::logic_error.*/
const double& Varying::floatValue() const
{
  assertTypeMatch(m_type, VaryingDataType::FLOAT);

  return m_data->floatValue();
}

// ###################################################################
/**Returns a string value for the value.*/
std::string Varying::convertToString(const bool with_type /*=false*/) const
{
  std::stringstream outstr; ///< output stream

  if (this->type() == VaryingDataType::ARBITRARY_BYTES)
  {
    const auto byte_values = this->m_data->bytesValue();
    for (const auto& value : byte_values)
    {
      const auto int_val = static_cast<unsigned int>(value);
      outstr << std::bitset<8>(int_val).to_string();
      if (value != byte_values.back())
        outstr << ",";
    }
    outstr << (with_type ? "(bytes)" : "");
  }
  else if (this->type() == VaryingDataType::STRING)
    outstr << "\"" << this->stringValue() << "\"" << (with_type ? "(string)" : "");
  else if (this->type() == VaryingDataType::FLOAT)
    outstr << this->floatValue() << (with_type ? "(double)" : "");
  else if (this->type() == VaryingDataType::INTEGER)
    outstr << this->integerValue() << (with_type ? "(integer)" : "");
  else if (this->type() == VaryingDataType::BOOL)
    outstr << (this->boolValue() ? "true" : "false") << (with_type ? "(bool)" : "");
  else
    outstr << "NONE";

  return outstr.str();
}

// ###################################################################
/**Determines if the value held is convertible to the target type.*/
bool Varying::isConvertibleToType(const VaryingDataType target_type) const
{
  switch (m_type)
  {
    // Only a void can be converted to a void
    case VaryingDataType::VOID:
      switch (target_type)
      {
        case VaryingDataType::VOID:
          return true;
        default:
          return false;
      } // switch target_type

    // Arbitrary bytes represent a black box.
    case VaryingDataType::ARBITRARY_BYTES:
      return false;

    // Sometimes a string can be converted to numbers
    case VaryingDataType::STRING:
      switch (target_type)
      {
        case VaryingDataType::VOID:
          return false;
        case VaryingDataType::ARBITRARY_BYTES:
        case VaryingDataType::BOOL:
        {
          const auto string_value = m_data->stringValue();
          return string_value == "true" or string_value == "false";
        }
        case VaryingDataType::STRING:
          return true;
        case VaryingDataType::INTEGER:
        case VaryingDataType::FLOAT:
        {
          return elke::string_utils::isStringANumber(this->stringValue());
        }
        default:
          return false;
      } // switch target_type

    // Bools, integers and floats can generally be converted to other things
    case VaryingDataType::BOOL:
    case VaryingDataType::INTEGER:
    case VaryingDataType::FLOAT:
      switch (target_type)
      {
        case VaryingDataType::VOID:
          return false;
        default:
          return true;
      } // switch target_type

    default:
      return false;
  } // switch (m_type)
}

} // namespace elke

// ###################################################################
/**Stream operator*/
std::ostream& operator<<(std::ostream& outstr, const elke::Varying& value)
{
  outstr << value.convertToString(false);
  return outstr;
}
