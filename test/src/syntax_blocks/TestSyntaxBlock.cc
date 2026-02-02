#include "TestSyntaxBlock.h"

#include "elke_core/registration/registration.h"

elkeRegisterSyntaxBlock(/*class_name=*/elke::TestSyntaxBlock,
                        /*block_syntax=*/TestSyntaxBlock);

namespace elke
{

ParameterTree TestSyntaxBlock::getInputParameters()
{
  auto params = ParameterTree("TestSyntaxBlock", "No description");

  params.addOptionalParameter("scale", "The scale", 2.0);
  params.addOptionalParameter("offset", "The offset", 2.0);
  params.addOptionalParameter("scale2", "The scale", 1.0);
  params.addRequiredParameter<int>("optionA", "Description.");

  auto& components = params.addOptionalParameter(
    "components", "A list of components to instantiate", GenericParameterMap());



  return params;
}

TestSyntaxBlock::TestSyntaxBlock(const ParameterTree& params)
  : SyntaxBlock(params)
// ,
//     m_scale(params.getParameterValue<double>("scale")),
//     m_delta(params.getParameterValue<double>("offset"))
{
}

} // namespace elke