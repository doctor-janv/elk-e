#include "CoreModule.h"

elkeRegisterModule(elke::CoreModule);

namespace elke
{

// ###################################################################
CoreModule::CoreModule() : ModuleBase("CoreModule") {};

// ###################################################################
CoreModule& CoreModule::getInstance() noexcept
{
  static auto instance = CoreModule();

  return instance;
}

// ###################################################################
ModuleBase& CoreModule::getBaseInstance()
{
  return static_cast<ModuleBase&>(getInstance());
}

} // namespace elke
