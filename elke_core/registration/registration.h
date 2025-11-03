#ifndef ELKE_CORE_REGISTRATION_REGISTRATION_H
#define ELKE_CORE_REGISTRATION_REGISTRATION_H

#include <vector>
#include <string>

/**Small utility macro for joining two words.*/
#define RJoinWordsA(x, y) x##y
/**IDK why this is needed. Seems like counter doesn't work properly without it*/
#define RJoinWordsB(x, y) RJoinWordsA(x, y)

/**Macro for registering a nullary/no-argument function to an app.*/
#define elkeRegisterNullaryFunction(func_name)                                 \
  static char RJoinWordsB(unique_var_name1_, __COUNTER__) =                    \
    elke::registerNullaryFunction(#func_name, func_name)

/**Macro for registering a module*/
#define elkeRegisterModule(module_name)                                        \
  static char RJoinWordsB(unique_var_name2_, __COUNTER__) =                    \
    elke::registerModule(#module_name, module_name::getBaseInstance)

namespace elke
{

class FrameworkCore;

using NullaryFunction = void (*)();

char registerNullaryFunction(const std::string& function_name,
                             NullaryFunction function);

} // namespace elke

#endif // ELKE_CORE_REGISTRATION_REGISTRATION_H
