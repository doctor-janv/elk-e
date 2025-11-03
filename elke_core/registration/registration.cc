#include "registration.h"
#include "elke_core/FrameworkCore.h"

#include <iostream>

namespace elke
{

// Registry& Registry::getInstance()
// {
//   static Registry instance;
//   return instance;
// }
//
// const std::vector<ModuleRegistryEntry>& Registry::getModuleRegistry() const
// {
//   return m_module_registry;
// }

char registerNullaryFunction(const std::string& function_name,
                             const NullaryFunction function)
{
  auto& core_module = FrameworkCore::getInstance();
  core_module.registerNullaryFunction(function_name, function);

  auto& logger = core_module.getLogger();

  logger.log() << "Registered Nullary function " << function_name
               << " to CoreModule";

  return 0;
}

// char registerModule(const std::string& module_name,
//                     const ModuleInstanceFetcher fetcher)
// {
//   auto& registry = Registry::getInstance();
//   const ModuleRegistryEntry reg_entry = {module_name, fetcher};
//   registry.m_module_registry.push_back(reg_entry);
//   std::cout << "Registered " << reg_entry.name << "\n";
//
//   return 0;
// }

} // namespace elke
