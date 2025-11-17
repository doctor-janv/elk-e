#ifndef ELKE_CORE_BASES_MODULEBASE_H
#define ELKE_CORE_BASES_MODULEBASE_H

#include "elke_core/mpi/MPI_Interface.h"
#include "elke_core/output/OutputInterface.h"
#include "elke_core/cli/CommandLineInterface.h"
#include "elke_core/base/Warehouse.h"
#include "elke_core/registration/registration.h"

#include <map>
#include <string>

namespace elke
{

/**The FrameworkCore is the center of entry points and operations. All objects
 *can interface with this singleton.*/
class FrameworkCore final : public MPI_Interface,
                            public OutputInterface,
                            public CommandLineInterface
{
  int m_argc = 0;
  char** m_argv = nullptr;
  bool m_initialized = false;

  int m_error_code = 0;

  std::map<std::string, NullaryFunction> m_nullary_function_register;

  /**Pre-input warehouse of objects.*/
  Warehouse m_warehouse;

  // Constructors/Destructors
  /**Private constructor*/
  FrameworkCore();
public:
  /**Destructor.*/
  ~FrameworkCore() = default;

  // Interface
  /**Obtains an instance to the ModuleBase singleton.*/
  static FrameworkCore& getInstance();

  /**Initializes the core.*/
  static void initialize(int argc, char** argv);

  /**Executes the Core module.*/
  static int execute();

  /**Registers a Nullary Function (no argument function). */
  void registerNullaryFunction(const std::string& function_name,
                               NullaryFunction function);

  /**Returns the mapping of nullary functions.*/
  const std::map<std::string, NullaryFunction>& getNullaryFunctions() const;

  /**Forcibly quits execution by throwing `std::runtime_error`.*/
  static void ForcedQuit(const std::string& reason="");

  /**Returns a constant reference to the warehouse.*/
  const Warehouse& warehouse() const;

  /**Returns a reference to the warehouse.*/
  Warehouse& warehouse();

protected:
  /**Registers CLI items specific to the CoreModule.*/
  void registerFrameworkCoreSpecificCLI();

  /**Responds to CoreModule specific CLAs*/
  void respondToFrameworkCoreCLAs() const;
  void basicCommandCall(const std::string& command_string) const;
  void dumpRegistry() const;
};

} // namespace elke

#endif // ELKE_CORE_BASES_MODULEBASE_H
