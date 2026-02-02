#include "SyntaxBlock.h"

#include "elke_core/parameters2/ParameterTree.h"

namespace elke
{

ParameterTree SyntaxBlock::getInputParameters()
{
  return ParameterTree(/*name=*/"SyntaxBlock",
                       /*description=*/"A syntax blocks is used to provide a "
                                       "sense of categorization to "
                                       "input data.");
}

SyntaxBlock::SyntaxBlock(const ParameterTree&) {}

} // namespace elke
