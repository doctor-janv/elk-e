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
const std::map<std::string, FactoryObjectRegisterEntry>&
StaticRegister::getFactoryObjectRegister()
{
  auto& registry = getInstance();
  return registry.m_factory_object_register;
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

// ###################################################################
const std::map<std::string, NamedParameterTreeRegistryEntry>&
StaticRegister::getInputParameterBlockRegistry()
{
  auto& registry = getInstance();
  return registry.m_input_blocks_register;
}

} // namespace elke
