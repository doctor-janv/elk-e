#include "ScalarValue.h"

#include "elke_core/output/elk_exceptions.h"
#include "elke_core/utilities/string_utils.h"

#include <set>
#include <sstream>

namespace elke
{

/**Returns a string representation of ScalarType.
 * \param type The type to convert to a string.
 */
std::string scalarTypeStringName(const ScalarType type)
{
  switch (type)
  {
    case ScalarType::VOID:
      return "VOID";
    case ScalarType::STRING:
      return "STRING";
    case ScalarType::BOOL:
      return "BOOL";
    case ScalarType::INTEGER:
      return "INTEGER";
    case ScalarType::FLOAT:
      return "FLOAT";
    default:
      return "VOID";
  }
}

/**Determines if the value held is convertible to the target type.
 * \param target_type The type to which the conversion is intended.
 */
bool ScalarValue::isConvertibleToType(const ScalarType target_type) const
{
  switch (m_scalar_type)
  {
      // clang-format off
    // Only a void can be converted to a void
    case ScalarType::VOID: return target_type == ScalarType::VOID;

      // Sometimes a string can be converted to numbers
    case ScalarType::STRING:
    {
      const auto string_value = std::any_cast<std::string>(m_value);
      if (target_type == ScalarType::STRING) return true;
      if (target_type == ScalarType::BOOL)
        return string_value == "true" or string_value == "false";

      if (target_type == ScalarType::INTEGER or
          target_type == ScalarType::FLOAT)
        return string_utils::isStringANumber(string_value);

      return false;

    }// case STRING

      // Bools, integers and floats can generally be converted to all others
      // except void
    case ScalarType::BOOL:
    case ScalarType::INTEGER:
    case ScalarType::FLOAT:
      return target_type != ScalarType::VOID;

    default: return false;
      // clang-format on
  } // switch (m_type)
}

/**Returns a new scalar value of this value converted to the target type.*/
ScalarValue ScalarValue::convertedToType(const ScalarType target_type) const
{

  switch (m_scalar_type)
  {
    // clang-format off
    // Only a void can be converted to a void
    case ScalarType::VOID:
      if (target_type == ScalarType::VOID) return *this;
      break;

      // Sometimes a string can be converted to numbers
    case ScalarType::STRING:
    {
      const auto string_value = std::any_cast<std::string>(m_value);
      if (target_type == ScalarType::STRING) return *this;

      if (target_type == ScalarType::BOOL)
      {
        if (string_value == "true") return ScalarValue(true);
        if (string_value == "false") return ScalarValue(false);
        break;
      }

      if (target_type == ScalarType::INTEGER)
      {
        if (not string_utils::isStringANumber(string_value)) break;
        const int64_t int_value = std::stoull(string_value);
        return ScalarValue(int_value);
      }

      if (target_type == ScalarType::FLOAT)
      {
        if (not string_utils::isStringANumber(string_value)) break;
        const double float_value = std::stod(string_value);
        return ScalarValue(float_value);
      }

      break;
    }// case STRING

      // Bools, integers and floats can generally be converted to all others
      // except void
    case ScalarType::BOOL:
    {
      const auto bool_value = std::any_cast<bool>(m_value);
      if (target_type == ScalarType::VOID) break;
      if (target_type == ScalarType::STRING)
        return bool_value ? ScalarValue("true") : ScalarValue("false");
      if (target_type == ScalarType::BOOL) return *this;
      if (target_type == ScalarType::INTEGER)
        return bool_value ? ScalarValue(1) : ScalarValue(0);
      if (target_type == ScalarType::FLOAT)
        return bool_value ? ScalarValue(1.0) : ScalarValue(0.0);

      break;
    }
    case ScalarType::INTEGER:
    {
      const auto int_value = std::any_cast<int64_t>(m_value);
      if (target_type == ScalarType::VOID) break;
      if (target_type == ScalarType::STRING)
        return ScalarValue(std::to_string(int_value));
      if (target_type == ScalarType::INTEGER) return *this;
      if (target_type == ScalarType::FLOAT)
      {
        const double float_value = int_value;
        return ScalarValue(float_value);
      }

      break;
    }
    case ScalarType::FLOAT:
    {
      const auto float_value = std::any_cast<double>(m_value);
      if (target_type == ScalarType::VOID) break;
      if (target_type == ScalarType::STRING)
        return ScalarValue(std::to_string(float_value));
      if (target_type == ScalarType::INTEGER)
      {
        const int64_t int_value = float_value;
        return ScalarValue(int_value);
      }
      if (target_type == ScalarType::FLOAT) return *this;

      break;
    }

    default: break;
    // clang-format on
  } // switch (m_type)

  elkLogicalError("Conversion failed to target type " +
                  scalarTypeStringName(target_type) + " for value " +
                  this->convertToString() + " of type " + this->typeString() +
                  ".");
}

/**Converts the underlying value to a string using a stream operator.
 * \param with_type If true, will append the type to the back of the string,
 *                  for example "12" becomes "12(INTEGER)".
 */
std::string ScalarValue::convertToString(const bool with_type) const
{
  std::stringstream outstr;

  switch (m_scalar_type)
  {
    case ScalarType::STRING:
    {
      const auto value = std::any_cast<std::string>(m_value);
      outstr << value + (with_type ? typeString() : "");
      break;
    }
    case ScalarType::BOOL:
    {
      const auto bool_value = std::any_cast<bool>(m_value);
      const std::string value = bool_value ? "true" : "false";
      outstr << value + (with_type ? typeString() : "");
      break;
    }
    case ScalarType::INTEGER:
    {
      const auto value = std::any_cast<int64_t>(m_value);
      outstr << value << (with_type ? typeString() : "");
      break;
    }
    case ScalarType::FLOAT:
    {
      const auto value = std::any_cast<double>(m_value);
      outstr << value << (with_type ? typeString() : "");
      break;
    }
    case ScalarType::VOID:
    default:
      outstr << "NONE" + (with_type ? typeString() : "");
      break;
  }

  return outstr.str();
}

} // namespace elke

std::ostream& operator<<(std::ostream& outstr, const elke::ScalarValue& value)
{
  outstr << value.convertToString(false);
  return outstr;
}
