#ifndef ELK_E_SYNTAXSYSTEM_H
#define ELK_E_SYNTAXSYSTEM_H

namespace elke
{

class ParameterTree;

class SyntaxBlock
{
protected:
  static ParameterTree getInputParameters();
  explicit SyntaxBlock(const ParameterTree& block);
};

} // namespace elke

#endif // ELK_E_SYNTAXSYSTEM_H
