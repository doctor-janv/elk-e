#include <utility>

#include "ModuleBase.h"

namespace elke
{

// ###################################################################
void ModuleBase::registerNullaryFunction(const std::string& function_name,
                                         NullaryFunction function)
{
  m_nullary_function_register.insert(std::make_pair(function_name, function));
}

void ModuleBase::sayHi() const { std::cout << "Hi from " << m_name << "\n"; }

// ###################################################################
Logger& ModuleBase::getLogger() { return m_logger; }

// ###################################################################
const std::map<std::string, NullaryFunction>&
ModuleBase::getNullaryFunctions() const
{
  return m_nullary_function_register;
}

// ###################################################################
ModuleBase::ModuleBase(std::string name, const int rank)
  : m_logger(/*verbosity=*/1, /*rank=*/rank),
    m_name(std::move(name)),
    m_program_header(ModuleBase::setDefaultHeader())
{
}

std::string ModuleBase::setDefaultHeader()
{
  // clang-format off
  std::string header =
"*******************************************************************************\n"
"*                                 ELKE                                        *\n"
"*                    A computational physics library                          *\n"
"*                          for thermal-hydraulics                             *\n"
"*******************************************************************************\n";
  // clang-format on
  return header;
}

} // namespace elke
