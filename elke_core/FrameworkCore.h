#ifndef ELKE_CORE_BASES_MODULEBASE_H
#define ELKE_CORE_BASES_MODULEBASE_H

#include "elke_core/mpi/MPI_Interface.h"
#include "elke_core/output/OutputInterface.h"
#include "elke_core/cli/CommandLineInterface.h"
#include "elke_core/base/Warehouse.h"
#include "elke_core/factory/Factory.h"
#include "elke_core/registration/registration.h"
#include "elke_core/input/InputProcessor.h"

#include <map>
#include <string>

namespace elke
{

class SimulationBlock;

/**The FrameworkCore is the center of entry points and operations. All objects
 *can interface with this singleton.*/
class FrameworkCore final : public MPI_Interface,
                            public OutputInterface
{
  /**Command line argument count.*/
  int m_argc;
  /**Command line argument values.*/
  char** m_argv;

  /**Saved execution error code.*/
  int m_error_code = 0;

  /**Flag indicating a non-error quit exception*/
  bool m_non_error_quit = false;

  /**Main command line interface.*/
  elke::CommandLineInterface m_CLI;

  /**Main input processor.*/
  elke::InputProcessor m_input_processor;

  /**Pre-input warehouse of objects.*/
  elke::Warehouse m_warehouse;

  /**Factory for creating objects.*/
  elke::Factory m_factory;

  /**This variable gets set to nullptr at runtime/libstart.*/
  static std::unique_ptr<FrameworkCore> m_instance_ptr;

  /**As the name suggests, the program will exit at the conclusion of this task
   * name.*/
  std::string m_task_at_which_to_stop;

  // Constructors/Destructors
  /**Private constructor*/
  explicit FrameworkCore(MPI_Comm communicator, int argc, char** argv);

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

private:
  /**Executes a named task by notifying all subscribers.*/
  void executeTask(const std::string& task_name) const;

public:
  /**Forcibly quits execution by throwing `std::runtime_error`.*/
  static void forcedQuit(const std::string& reason = "");

  /**Given a user condition a quit has been initiated.*/
  static void userMarkedQuit(const std::string& reason = "");

  /**Returns a constant reference to the warehouse.*/
  const Warehouse& warehouse() const;

  /**Returns a reference to the warehouse.*/
  Warehouse& warehouse();

  /**Returns the supplied command line arguments.*/
  const CommandLineArgumentList& getSuppliedCommandLineArguments() const
  {
    return m_CLI.getSuppliedCommandLineArguments();
  }

  InputProcessor& inputProcessor()
  {
    return m_input_processor;
  }

protected:
  /**Registers CLI items specific to the CoreModule.*/
  void registerFrameworkCoreSpecificCLI();

  /**Responds to CoreModule specific CLAs*/
  void respondToFrameworkCoreCLAs();
  void basicCommandCall(const std::string& command_string) const;
  void dumpRegistry() const;
};

} // namespace elke

#endif // ELKE_CORE_BASES_MODULEBASE_H
