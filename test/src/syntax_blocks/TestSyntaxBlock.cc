
#include "elke_core/parameters/InputParametersBlock.h"
#include "elke_core/registration/registration.h"

namespace elke
{

/**A test syntax block to test very robustness related aspects.*/
class TestSyntaxBlock : public SyntaxBlock
{
  double m_scale;
  double m_delta;
public:
  static InputParametersBlock getInputParameters();
  explicit TestSyntaxBlock(const InputParametersBlock& params);
};

} // namespace elke

elkeRegisterSyntaxBlock(/*class_name=*/elke::TestSyntaxBlock,
                        /*block_syntax=*/TestSyntaxBlock);

namespace elke
{

InputParametersBlock TestSyntaxBlock::getInputParameters()
{
  auto params = InputParametersBlock(
    "TestSyntaxBlock", "No description", {SyntaxBlock::getInputParameters()});

  params.addOptionalParameter("scale", 1.0, "The scale");
  params.addOptionalParameter("offset", 2.0, "The offset",
    {std::make_shared<param_checks::ScalarTypeMustMatch>()});
  params.addOptionalParameter("scale2", 1.0, "The scale",
    {std::make_shared<param_checks::ScalarTypeMustMatch>()});
  params.addRequiredParameter<int>("optionA", "Description.");

  return params;
}

TestSyntaxBlock::TestSyntaxBlock(const InputParametersBlock& params)
  : SyntaxBlock(params),
    m_scale(params.getParameterValue<double>("scale")),
    m_delta(params.getParameterValue<double>("offset"))
{
}

} // namespace elke