#ifndef ELK_E_INPUTPARAMETERS_H
#define ELK_E_INPUTPARAMETERS_H

#include "InputParameter.h"
#include <vector>

namespace elke
{

class InputParametersBlock
{
  std::vector<InputParameter> m_parameters;
  std::string m_block_description;

public:
  /**Creates a barebones empty block.*/
  InputParametersBlock() = default;

  void setBlockDescription(std::string block_description);

  template <typename T>
  void addOptionalParameter(std::string name,
                            T default_value,
                            std::string description)
  {
    m_parameters.emplace_back(ScalarInputParameter(
      name, ParameterClass::OPTIONAL, default_value, description));
  }

  template <typename T>
  void addRequiredParameter(std::string name, std::string description)
  {
    m_parameters.emplace_back(
      ScalarInputParameter<T>(name, ParameterClass::REQUIRED, description));
  }

  const InputParameter& getParameter(const std::string& name) const;

  template<typename T>
  T getParameterValue(const std::string& name) const
  {
    const auto& param = getParameter(name);

    return param.getValue<T>();
  }
};

} // namespace elke

#endif // ELK_E_INPUTPARAMETERS_H
