#include "InputParametersBlock.h"

#include "elke_core/output/elk_exceptions.h"

namespace elke
{

void InputParametersBlock::setBlockDescription(std::string block_description)
{
  m_block_description = std::move(block_description);
}

const InputParameter&
InputParametersBlock::getParameter(const std::string& name) const
{
  for (const auto& parameter : m_parameters)
    if (parameter.name() == name) return parameter;

  elkLogicalError("InputParameterBlock has no parameter named \"" + name +
                  "\"");
}

} // namespace elke