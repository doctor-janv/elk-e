#include "SimulationBlock.h"

#include "elke_core/parameters2/ParameterTree.h"
#include "elke_core/registration/registration.h"

elkeRegisterSyntaxBlock(/*class_name=*/elke::SimulationBlock,
                        /*block_syntax=*/Simulation);

namespace elke
{

ParameterTree SimulationBlock::getInputParameters()
{
  auto params = ParameterTree(
    "Simulation", "No description");

  params.addOptionalParameter("scale", "The scale", 2.0);
  params.addOptionalParameter("offset", "The offset", 2.0);
  params.addOptionalParameter("scale2", "The scale", 1.0);
  params.addRequiredParameter<int>("optionA", "Description.");

  return params;
}

SimulationBlock::SimulationBlock(const ParameterTree& params)
  : SyntaxBlock(params)
// ,
//     m_scale(params.getParameterValue<double>("scale")),
//     m_delta(params.getParameterValue<double>("offset"))
{
}

} // namespace elke