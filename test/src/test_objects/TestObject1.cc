#include "elke_core/factory/FactoryObject.h"
#include "elke_core/registration/registration.h"

namespace elke
{

class TestObject1 : public FactoryObject
{
  const int m_num_refinements;

public:
  static InputParametersBlock getInputParameters();
  explicit TestObject1(const InputParametersBlock& input_parameters);
};

InputParametersBlock TestObject1::getInputParameters()
{
  auto params = InputParametersBlock("elke::TestObject1",
                                     "A test object",
                                     {FactoryObject::getInputParameters()});
  params.addRequiredParameter<int>("num_refinements", "Number of refinements");

  return params;
}

TestObject1::TestObject1(const InputParametersBlock& input_parameters)
  : FactoryObject(input_parameters),
    m_num_refinements(
      input_parameters.getParameterValue<int>("num_refinements"))
{
}

} // namespace elke

elkeRegisterObject(elke::TestObject1);
