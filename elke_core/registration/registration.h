#ifndef ELKE_CORE_REGISTRATION_REGISTRATION_H
#define ELKE_CORE_REGISTRATION_REGISTRATION_H

#include <vector>
#include <string>

/**Small utility macro for joining two words.*/
#define RJoinWordsA(x, y) x##y
/**IDK why this is needed. Seems like counter doesn't work properly without it*/
#define RJoinWordsB(x, y) RJoinWordsA(x, y)

/**Macro for registering a nullary/no-argument function to an app.*/
#define elkeRegisterNullaryFunction(app_name, func_name)                       \
  static char RJoinWordsB(unique_var_name1_, __COUNTER__) =                    \
    elke::registerNullaryFunction(                                             \
      #app_name, app_name::getInstance(), #func_name, func_name)

/**Macro for registering a module*/
#define elkeRegisterModule(module_name)                                        \
  static char RJoinWordsB(unique_var_name2_, __COUNTER__) =                    \
    elke::registerModule(#module_name, module_name::getBaseInstance)

namespace elke
{

class ModuleBase;

using NullaryFunction = void (*)();
using ModuleInstanceFetcher = ModuleBase& (*)();

struct ModuleRegistryEntry
{
  const std::string name;
  ModuleInstanceFetcher fetcher;
};

char registerNullaryFunction(const std::string& module_name,
                             ModuleBase& module,
                             const std::string& function_name,
                             NullaryFunction function);

char registerModule(const std::string& module_name,
                    ModuleInstanceFetcher fetcher);

//###################################################################
/**This is the global registry singleton.*/
class Registry
{
  friend char registerModule(const std::string& module_name,
                    ModuleInstanceFetcher fetcher);
public:
  static Registry& getInstance();

  const std::vector<ModuleRegistryEntry>& getModuleRegistry() const;

private:
  Registry() = default;

  std::vector<ModuleRegistryEntry> m_module_registry;
};

} // namespace elke

#endif // ELKE_CORE_REGISTRATION_REGISTRATION_H
