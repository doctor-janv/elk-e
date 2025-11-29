#include "Varying.h"

#include <sstream>

#include "elke_core/output/elk_exceptions.h"

/**Checks if two VaryingDataType values match.
 * Type A is matched against type B. Throws std::logic_error if not.*/
void elke::Varying::AssertTypeMatch(const VaryingDataType type_A,
                                    const VaryingDataType type_B_required) const
{
  if (type_A != type_B_required)
    throw std::logic_error("Varying data type " + TypeName() +
                           " does not "
                           "correspond to the required type, " +
                           VaryingDataTypeStringName(type_B_required));
}

// ###################################################################
// Constructors

/**Constructor for an arbitrary sequence of bytes value.*/
elke::Varying::Varying(const std::vector<std::byte>& value)
  : m_type(VaryingDataType::ARBITRARY_BYTES)
{
  m_data =
    std::make_unique<VaryingTemplatedData<std::vector<std::byte>>>(value);
}

/**Constructor for a string value.*/
elke::Varying::Varying(const std::string& value)
  : m_type(VaryingDataType::STRING),
    m_data(std::make_unique<VaryingTemplatedData<std::string>>(value))
{
}

/**Copy constructor.*/
elke::Varying::Varying(const Varying& other)
{
  m_data = other.m_data == nullptr ? nullptr : other.m_data->Clone();
  m_type = other.m_type;
}

/**Move constructor.*/
elke::Varying::Varying(Varying&& other) noexcept
{
  std::swap(m_data, other.m_data);
  std::swap(m_type, other.m_type);
}

/**Assignment operator. i.e., type_A = type_B*/
elke::Varying& elke::Varying::operator=(const Varying& other)
{
  if (this != &other)
  {
    m_data = other.m_data->Clone();
    m_type = other.m_type;
  }
  return *this;
}

// ###################################################################
//  Assignments
/**Assigns an arbitrary sequence of bytes value.*/
elke::Varying& elke::Varying::operator=(const std::vector<std::byte>& value)
{
  m_type = VaryingDataType::ARBITRARY_BYTES;
  m_data =
    std::make_unique<VaryingTemplatedData<std::vector<std::byte>>>(value);
  return *this;
}

/**Assigns a string value.*/
elke::Varying& elke::Varying::operator=(const std::string& value)
{
  m_type = VaryingDataType::STRING;
  m_data = std::make_unique<VaryingTemplatedData<std::string>>(value);
  return *this;
}

// ###################################################################
//  Get values
/**Returns the string value if valid. Otherwise, throws std::logic_error.*/
std::string elke::Varying::StringValue() const
{
  AssertTypeMatch(m_type, VaryingDataType::STRING);

  return m_data->StringValue();
}

/**Returns the bool value if valid. Otherwise, throws std::logic_error.*/
bool elke::Varying::BoolValue() const
{
  AssertTypeMatch(m_type, VaryingDataType::BOOL);

  return m_data->BoolValue();
}

/**Returns the integer value if valid. Otherwise, throws std::logic_error.*/
int64_t elke::Varying::IntegerValue() const
{
  AssertTypeMatch(m_type, VaryingDataType::INTEGER);

  return m_data->IntegerValue();
}

/**Returns the float value if valid. Otherwise, throws std::logic_error.*/
double elke::Varying::FloatValue() const
{
  AssertTypeMatch(m_type, VaryingDataType::FLOAT);

  return m_data->FloatValue();
}

// ###################################################################
/**Returns the raw byte size associated with the type.*/
size_t elke::Varying::ByteSize() const { return m_data->Size(); }

// ###################################################################
/**Returns a string value for the value.*/
std::string elke::Varying::PrintStr(const bool with_type /*=false*/) const
{
  std::stringstream outstr;

  if (this->Type() == VaryingDataType::STRING)
    outstr << "\"" << this->StringValue() << "\"";
  else if (this->Type() == VaryingDataType::FLOAT)
    outstr << this->FloatValue() << (with_type ? "(double)" : "");
  else if (this->Type() == VaryingDataType::INTEGER)
    outstr << this->IntegerValue();
  else if (this->Type() == VaryingDataType::BOOL)
    outstr << (this->BoolValue() ? "true" : "false");
  else
    outstr << "NONE";

  return outstr.str();
}

// ###################################################################
/**Stream operator*/
std::ostream& operator<<(std::ostream& outstr, const elke::Varying& value)
{
  outstr << value.PrintStr(false);
  return outstr;
}
