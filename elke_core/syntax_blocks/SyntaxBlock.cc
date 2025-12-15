#include "SyntaxBlock.h"

#include "elke_core/parameters/InputParametersBlock.h"

namespace elke
{

InputParametersBlock SyntaxBlock::getInputParameters()
{
  return {"SyntaxBlock",
          "A syntax blocks is used to provide a sense of categorization to "
          "input data."};
}

SyntaxBlock::SyntaxBlock(const InputParametersBlock& block) {}

} // namespace elke
