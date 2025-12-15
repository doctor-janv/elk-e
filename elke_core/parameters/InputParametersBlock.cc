#include "InputParametersBlock.h"

#include <utility>

#include "elke_core/output/elk_exceptions.h"

namespace elke
{

InputParametersBlock::InputParametersBlock(
  std::string name,
  std::string description,
  const std::vector<InputParametersBlock>& parent_blocks /*={}*/)
  : m_name(std::move(name)), m_block_description(std::move(description))
{
  for (const auto& parent_block : parent_blocks)
    for (const auto& parameter : parent_block)
      if (not this->hasParameter(parameter.name()))
        this->m_parameters.push_back(
          std::make_shared<InputParameter>(parameter));
      else
      {
        throw std::runtime_error(
          "While adding parameters from parent block \"" + parent_block.name() +
          "\" to block \"" + name + "\", parameter \"" + parameter.name() +
          "\" is already added and cannot be duplicated.");
      }
}

const InputParameter&
InputParametersBlock::getParameter(const std::string& name) const
{
  for (const auto& parameter_ptr : m_parameters)
    if (parameter_ptr->name() == name) return *parameter_ptr;

  elkLogicalError("InputParameterBlock has no parameter named \"" + name +
                  "\"");
}

} // namespace elke