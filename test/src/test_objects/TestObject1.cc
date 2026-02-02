#include "elke_core/factory/FactoryObject.h"
#include "elke_core/registration/registration.h"

namespace elke
{

class TestObject1 : public FactoryObject
{
  // const int m_num_refinements;
  int m_num_refinements;

public:
  static ParameterTree getInputParameters();
  explicit TestObject1(const ParameterTree& input_parameters);
};

ParameterTree TestObject1::getInputParameters()
{
  auto params = ParameterTree("elke::TestObject1",
                                     "A test object"
                                     // ,
                                     // {FactoryObject::getInputParameters()}
                                     );
  params.addRequiredParameter<int>("num_refinements", "Number of refinements");

  return params;
}

TestObject1::TestObject1(const ParameterTree& input_parameters)
  : FactoryObject(input_parameters)
// ,
//     m_num_refinements(
//       input_parameters.getParameterValue<int>("num_refinements"))
{
}

} // namespace elke

elkeRegisterObject(elke::TestObject1);
