#include "Varying.h"

#include <sstream>

#include "elke_core/output/elk_exceptions.h"

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
/**Returns the string value if valid. Otherwise, throws std::logic_error.*/
std::string Varying::stringValue() const
{
  assertTypeMatch(m_type, VaryingDataType::STRING);

  return m_data->stringValue();
}

/**Returns the bool value if valid. Otherwise, throws std::logic_error.*/
bool Varying::boolValue() const
{
  assertTypeMatch(m_type, VaryingDataType::BOOL);

  return m_data->boolValue();
}

/**Returns the integer value if valid. Otherwise, throws std::logic_error.*/
int64_t Varying::integerValue() const
{
  assertTypeMatch(m_type, VaryingDataType::INTEGER);

  return m_data->integerValue();
}

/**Returns the float value if valid. Otherwise, throws std::logic_error.*/
double Varying::floatValue() const
{
  assertTypeMatch(m_type, VaryingDataType::FLOAT);

  return m_data->floatValue();
}

// ###################################################################
/**Returns a string value for the value.*/
std::string Varying::convertToString(const bool with_type /*=false*/) const
{
  std::stringstream outstr; ///< output stream

  if (this->type() == VaryingDataType::STRING)
    outstr << "\"" << this->stringValue() << "\"";
  else if (this->type() == VaryingDataType::FLOAT)
    outstr << this->floatValue() << (with_type ? "(double)" : "");
  else if (this->type() == VaryingDataType::INTEGER)
    outstr << this->integerValue();
  else if (this->type() == VaryingDataType::BOOL)
    outstr << (this->boolValue() ? "true" : "false");
  else
    outstr << "NONE";

  return outstr.str();
}

} // namespace elke

// ###################################################################
/**Stream operator*/
std::ostream& operator<<(std::ostream& outstr, const elke::Varying& value)
{
  outstr << value.convertToString(false);
  return outstr;
}
