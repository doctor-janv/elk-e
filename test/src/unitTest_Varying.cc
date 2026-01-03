#include <elke_core/FrameworkCore.h>

#include "elke_core/data_types/Varying.h"
#include "elke_core/utilities/general_utils.h"

#include "elke_core/data_types/ScalarData.h"

namespace elke::unit_tests
{

void unitTestVarying()
{
  const auto& core = FrameworkCore::getInstance();
  auto& logger = core.getLogger();

  //======================================================= Basic constructors
  //                                                        and printing
  {
    const auto byte_array = std::vector{std::byte{0x10}, std::byte{0x20}};

    const auto v0 = Varying();           // Creates a VOID type
    const auto v1 = Varying(byte_array); // Creates a BYTE_ARRAY type
    const auto v2 = Varying("Name");     // Creates a STRING type
    const auto v3 = Varying(12);         // Creates an INTEGER type
    const auto v4 = Varying(1.2);        // Creates a FLOAT type

    logger.log() << "v0 " << v0.convertToString(true) << "\n";
    logger.log() << "v1 " << v1.convertToString(true) << "\n";
    logger.log() << "v2 " << v2.convertToString(true) << "\n";
    logger.log() << "v3 " << v3.convertToString(true) << "\n";
    logger.log() << "v4 " << v4.convertToString(true) << "\n";
  }

  //======================================================= Byte array conversion
  {
    struct ComplexData
    {
      int x = 0;
      double y = 0;
      size_t z = 0;

      std::string toString() const
      {
        std::stringstream ss;
        ss << "x=" << x << ", y=" << y << ", z=" << z;
        return ss.str();
      }
    };
    constexpr auto data1 = ComplexData{11,1.2, 1100};

    const auto v0 = Varying(elke::toBytes(data1));

    ComplexData data2;
    logger.log() << "data2 before: " << data2.toString();
    data2 = elke::from_bytes<ComplexData>(v0.bytesValue());
    logger.log() << "data2 after: " << data2.toString();

  }

  //======================================================= Scalar type
  {
    logger.log() << "New ScalarData tests";
    const auto byte_array = std::vector{std::byte{0x10}, std::byte{0x20}};

    const auto v0 = ScalarData();           // Creates a VOID type
    const auto v1 = ScalarData(byte_array); // Creates a BYTE_ARRAY type
    const auto v2 = ScalarData("Name");     // Creates a STRING type
    const auto v2a = ScalarData(std::string("Name"));
    const auto v3 = ScalarData(12);         // Creates an INTEGER type
    const auto v4 = ScalarData(1.2);        // Creates a FLOAT type

    auto str_ref = ScalarData("Name");
    logger.log() << "str_ref=" << str_ref.getValue<std::string>();
    auto& val = str_ref.getValue<std::string>();
    val += "2";
    logger.log() << "str_ref=" << str_ref.getValue<std::string>();
  }

}

} // namespace elke::unit_tests

elkeRegisterNullaryFunction(elke::unit_tests::unitTestVarying);