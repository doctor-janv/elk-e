#include "FrameworkCore.h"

#include "elke_core/output/elk_exceptions.h"

namespace elke
{

std::unique_ptr<FrameworkCore> FrameworkCore::m_instance_ptr = nullptr;

// ###################################################################
FrameworkCore::FrameworkCore(const MPI_Comm communicator,
                             const int argc,
                             char** argv)
  : MPI_Interface(communicator),
    OutputInterface(this->rank()),
    m_argc(argc),
    m_argv(argv),
    m_CLI(this->getLoggerPtr()),
    m_input_processor(this->getLoggerPtr()),
    m_factory(m_warehouse)
{
}

// ###################################################################
FrameworkCore& FrameworkCore::getInstance()
{
  elkLogicalErrorIf(m_instance_ptr == nullptr,
                    "This method was called before a call to "
                    "elke::FrameworkCore::initialize was made.");

  return *m_instance_ptr;
}

// ###################################################################
void FrameworkCore::initialize(const int argc, char** argv)
{
#ifdef MPI_VERSION
  MPI_Init(&argc, &argv); /* starts MPI */
#endif

  // In the code below we cannot use std::make_unique because the constructor
  // is private.
  m_instance_ptr = std::unique_ptr<FrameworkCore>(
    new FrameworkCore(MPI_COMM_WORLD, argc, argv));
  auto& core = *m_instance_ptr;

#ifdef MPI_VERSION
  auto header = core.getHeader();

  header += "MPI-Version: ";
  header += std::to_string(MPI_VERSION) + ".";
  header += std::to_string(MPI_SUBVERSION) + ".";
  header += std::to_string(0);

  core.setHeader(header);
#endif
}

// ###################################################################
int FrameworkCore::execute()
{
  auto& core = FrameworkCore::getInstance();
  try
  {
    core.m_CLI.registerCommonCLI_Items();
    core.registerFrameworkCoreSpecificCLI();
    core.executeTask("CLI_registration");

    core.m_CLI.parseCommandLine(core.m_argc, core.m_argv);
    core.m_CLI.respondToBasicCLAs();
    core.respondToFrameworkCoreCLAs();
    core.executeTask("respond_to_CLAs");

    core.m_input_processor.parseInputFiles();
    core.executeTask("input_parsing");

    core.m_input_processor.checkInputDataForSyntaxBlocks();
    core.executeTask("input_checking");
  }
  catch (const std::exception& exception_object)
  {
    if (core.m_non_error_quit)
    {
      core.getLogger().log() << exception_object.what() << "\n";
      return 0;
    }

    core.getLogger().error() << exception_object.what() << "\n";

    return core.m_error_code == 0 ? 1 : core.m_error_code;
  }

  return 0;
}

// ###################################################################
void FrameworkCore::executeTask(const std::string& task_name) const
{
  // Exit condition
  if (task_name == m_task_at_which_to_stop)
  {

    userMarkedQuit("Execution halted at end of requested task \"" + task_name +
                   "\"");
  }
}

// ###################################################################
void FrameworkCore::forcedQuit(const std::string& reason /*=""*/)
{
  auto& core = FrameworkCore::getInstance();
  core.m_error_code = 1;
  throw std::runtime_error("Quit has been called. " + reason);
}

// ###################################################################
void FrameworkCore::userMarkedQuit(const std::string& reason /*=""*/)
{
  auto& core = FrameworkCore::getInstance();
  core.m_error_code = 0;
  core.m_non_error_quit = true;
  throw std::runtime_error("Quit has been called. " + reason);
}

// ###################################################################
const Warehouse& FrameworkCore::warehouse() const { return m_warehouse; }

// ###################################################################
Warehouse& FrameworkCore::warehouse() { return m_warehouse; }

} // namespace elke

/**elke C-API call to initialize the FrameworkCore.*/
extern "C" void elke_FrameworkCore_initialize()
{
  elke::FrameworkCore::initialize(0, nullptr);
}