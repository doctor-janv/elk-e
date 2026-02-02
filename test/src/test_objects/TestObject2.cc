#include "elke_core/factory/FactoryObject.h"
#include "elke_core/registration/registration.h"

namespace elke
{

class TestObject2 : public FactoryObject
{
  // const int m_num_refinements;
  int m_num_refinements;

public:
  static ParameterTree getInputParameters();
  explicit TestObject2(const ParameterTree& input_parameters);
};

ParameterTree TestObject2::getInputParameters()
{
  auto params = ParameterTree("elke::TestObject1",
                                     "A test object"
                                     // ,
                                     // {FactoryObject::getInputParameters()}
                                     );
  params.addRequiredParameter<int>("num_refinements", "Number of refinements");
  // params.addRequiredParameter<RegisteredObjectProxy>("sub_object", "Yup");

  return params;
}

TestObject2::TestObject2(const ParameterTree& input_parameters)
  : FactoryObject(input_parameters)
// ,
//     m_num_refinements(
//       input_parameters.getParameterValue<int>("num_refinements"))
{
}

} // namespace elke

elkeRegisterObject(elke::TestObject2);
