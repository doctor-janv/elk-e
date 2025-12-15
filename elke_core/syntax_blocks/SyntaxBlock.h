#ifndef ELK_E_SYNTAXSYSTEM_H
#define ELK_E_SYNTAXSYSTEM_H

namespace elke
{

class InputParametersBlock;
class DataTree;

class SyntaxBlock
{
protected:
  static InputParametersBlock getInputParameters();
  explicit SyntaxBlock(const InputParametersBlock& block);
};

} // namespace elke

#endif // ELK_E_SYNTAXSYSTEM_H
