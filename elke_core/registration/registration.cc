#include "registration.h"
#include "elke_core/base/ModuleBase.h"

#include <iostream>

namespace elke
{

Registry& Registry::getInstance()
{
  static Registry instance;
  return instance;
}

const std::vector<ModuleRegistryEntry>& Registry::getModuleRegistry() const
{
  return m_module_registry;
}

char registerNullaryFunction(const std::string& module_name,
                             ModuleBase& module,
                             const std::string& function_name,
                             NullaryFunction function)
{
  module.registerNullaryFunction(function_name, function);

  std::cout << "Registered " << function_name << " to " << module_name << "\n";

  return 0;
}

char registerModule(const std::string& module_name,
                    const ModuleInstanceFetcher fetcher)
{
  auto& registry = Registry::getInstance();
  const ModuleRegistryEntry reg_entry = {module_name, fetcher};
  registry.m_module_registry.push_back(reg_entry);
  std::cout << "Registered " << reg_entry.name << "\n";

  return 0;
}

} // namespace elke
