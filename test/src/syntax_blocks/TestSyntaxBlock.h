#ifndef ELK_E_TESTSYNTAXBLOCK_H
#define ELK_E_TESTSYNTAXBLOCK_H

#include "elke_core/syntax_blocks/SyntaxBlock.h"


namespace elke
{

/**A test syntax block to test very robustness related aspects.*/
class TestSyntaxBlock : public SyntaxBlock
{
  double m_scale;
  double m_delta;
public:
  static ParameterTree getInputParameters();
  explicit TestSyntaxBlock(const ParameterTree& params);
};

} // namespace elke

#endif // ELK_E_TESTSYNTAXBLOCK_H
