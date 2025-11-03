#include "elke_core/math/Vec3.h"
#include "elke_core/FrameworkCore.h"

namespace elke::unit_tests
{

/**Routine to test basic functionality of Vec3 */
void unitTestVec3() { std::cout << "Hello from unitTestVec3\n"; }

} // namespace elke::unit_tests

elkeRegisterNullaryFunction(elke::unit_tests::unitTestVec3);
