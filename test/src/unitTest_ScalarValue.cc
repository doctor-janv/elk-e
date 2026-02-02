#include <elke_core/FrameworkCore.h>

#include "elke_core/data_types/ScalarValue.h"

namespace elke::unit_tests
{

void unitTestScalarValue()
{
  const auto& core = FrameworkCore::getInstance();
  auto& logger = core.getLogger();

  //======================================================= Blank constructors
  {
    logger.log() << "------------------------------ No value constructors.";

    const auto v0 = ScalarValue();        // Creates a VOID type
    const auto v1 = ScalarValue(std::string{}); // Creates a STRING type
    const auto v2 = ScalarValue(int{});  // Creates an INTEGER type
    const auto v3 = ScalarValue(float{}); // Creates a FLOAT type
  }

  //======================================================= Scalar type
  {
    logger.log() << "------------------------------ With-value constructors.";

    const auto v0 = ScalarValue();        // Creates a VOID type
    const auto v1a = ScalarValue("Name"); // Creates a STRING type from literal
    const auto v1b = ScalarValue(std::string("Name")); // Creates a STRING type
    const auto v2 = ScalarValue(12);  // Creates an INTEGER type
    const auto v3 = ScalarValue(1.2); // Creates a FLOAT type

    logger.log() << "v1a=" << v1a.getValue<std::string>();
  }

  {
    logger.log() << "------------------------------ Test references.";
    auto str_ref = ScalarValue("Name");
    logger.log() << "str_ref=" << str_ref.getValue<std::string>();
    auto& val = str_ref.getValue<std::string&>();
    val += "2";
    logger.log() << "str_ref=" << str_ref.getValue<std::string>();
  }
}

} // namespace elke::unit_tests

elkeRegisterNullaryFunction(elke::unit_tests::unitTestScalarValue);