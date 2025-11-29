#include "SimulationBlock.h"

#include "elke_core/parameters/InputParametersBlock.h"
#include "elke_core/registration/registration.h"

elkeRegisterSyntaxBlock(elke::SimulationBlock, Simulation);

namespace elke
{

InputParametersBlock SimulationBlock::getInputParameters()
{
  auto params = InputParametersBlock();

  params.addOptionalParameter<double>("scale", 1.0, "The scale");
  params.addOptionalParameter<double>("offset", 1.0, "The offset");

  return params;
}

SimulationBlock::SimulationBlock(const InputParametersBlock& params)
  : SyntaxBlock(params),
    m_scale(params.getParameterValue<double>("scale")),
    m_delta(params.getParameterValue<double>("offset"))
{
}

} // namespace elke