#ifndef ELK_E_EXECUTION_CONTEXT_H
#define ELK_E_EXECUTION_CONTEXT_H

#include "elke_core/syntax_blocks/SyntaxBlock.h"

namespace elke
{

/**A system to respond to [Simulation] top level tags.*/
class SimulationBlock : public SyntaxBlock
{
  double m_scale;
  double m_delta;
public:
  static ParameterTree getInputParameters();
  explicit SimulationBlock(const ParameterTree& params);
};

} // namespace elke

#endif // ELK_E_EXECUTION_CONTEXT_H
