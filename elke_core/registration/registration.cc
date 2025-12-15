#include "registration.h"
#include "elke_core/FrameworkCore.h"

namespace elke
{

StaticRegister& StaticRegister::getInstance()
{
  static auto instance_ptr =
    std::unique_ptr<StaticRegister>(new StaticRegister());

  return *instance_ptr;
}

// ###################################################################
const std::map<std::string, NullaryFunction>&
StaticRegister::getNullaryFunctions()
{
  auto& registry = getInstance();
  return registry.m_nullary_function_register;
}

// ###################################################################
const std::map<std::string, SyntaxBlockRegisterEntry>&
StaticRegister::getSyntaxSystemRegister()
{
  auto& registry = getInstance();
  return registry.m_syntax_block_register;
}


// ###################################################################
char StaticRegister::registerNullaryFunction(const std::string& function_name,
                                             NullaryFunction function)
{
  auto& registry = getInstance();

  registry.m_nullary_function_register.insert(
    std::make_pair(function_name, function));

  return 0;
}

} // namespace elke
