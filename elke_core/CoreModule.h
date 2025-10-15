#ifndef ELKE_CORE_COREMODULE_H
#define ELKE_CORE_COREMODULE_H

#include "elke_core/base/ModuleBase.h"
#include "elke_core/registration/registration.h"

namespace elke
{

/**The core module*/
class CoreModule final : public elke::ModuleBase
{
private:
  /**Constructor*/
  CoreModule();

public:
  /**Returns an instance to the module singleton.*/
  static CoreModule& getInstance() noexcept;
  /**Returns an instance to the base module singleton.*/
  static ModuleBase& getBaseInstance();

  /**Executes the module*/
  static int execute(int argc, char** argv);


private:
  void registerCLI();
  void initialCLIResponse();

};

} // namespace elke

#endif // ELKE_CORE_COREMODULE_H
