#include <utility>

#include "FrameworkCore.h"

namespace elke
{

// ###################################################################
FrameworkCore::FrameworkCore()
  : MPI_Interface(),
    OutputInterface(0),
    CommandLineInterface(this->getLoggerPtr())
{
}

// ###################################################################
FrameworkCore& FrameworkCore::getInstance()
{
  static FrameworkCore instance;

  return instance;
}

// ###################################################################
void FrameworkCore::initialize(const int argc, char** argv)
{
  auto& core = FrameworkCore::getInstance();

  core.m_argc = argc;
  core.m_argv = argv;

  core.reinitializeMPI_Interface(argc, argv, MPI_COMM_WORLD);
  core.getLogger().setRank(core.rank());
  auto header = core.getHeader();

  const auto mpi_version = core.getMPIVersion();
  header += "MPI-Version: ";
  header += std::to_string(mpi_version[0]) + ".";
  header += std::to_string(mpi_version[1]) + ".";
  header += std::to_string(mpi_version[2]);

  core.setHeader(header);

  core.m_initialized = true;
}

// ###################################################################
int FrameworkCore::execute()
{
  auto& core = FrameworkCore::getInstance();
  try
  {
    core.registerCommonCLI_Items();
    core.registerFrameworkCoreSpecificCLI();
    core.parseCommandLine(core.m_argc, core.m_argv);
    core.respondToCommonCLAs();
    core.respondToFrameworkCoreCLAs();
  }
  catch (const std::exception& exception_object)
  {
    core.getLogger().error() << exception_object.what() << "\n";
    return core.m_error_code;
  }

  return 0;
}

// ###################################################################
void FrameworkCore::registerNullaryFunction(const std::string& function_name,
                                            NullaryFunction function)
{
  m_nullary_function_register.insert(std::make_pair(function_name, function));
}

// ###################################################################
const std::map<std::string, NullaryFunction>&
FrameworkCore::getNullaryFunctions() const
{
  return m_nullary_function_register;
}

// ###################################################################
void FrameworkCore::ForcedQuit(const std::string& reason /*=""*/)
{
  auto& core = FrameworkCore::getInstance();
  core.m_error_code = 1;
  throw std::runtime_error("Quit has been called. " + reason);
}

// ###################################################################
const Warehouse& FrameworkCore::warehouse() const { return m_warehouse; }

// ###################################################################
Warehouse& FrameworkCore::warehouse() { return m_warehouse; }

} // namespace elke
