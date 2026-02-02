#ifndef ELKE_CORE_REGISTRATION_REGISTRATION_H
#define ELKE_CORE_REGISTRATION_REGISTRATION_H

#include "elke_core/parameters2/ParameterTree.h"
#include "elke_core/syntax_blocks/SyntaxBlock.h"
#include "elke_core/factory/FactoryObject.h"

#include <map>
#include <string>

/**Small utility macro for joining two words.*/
#define RJoinWordsA(x, y) x##y
/**IDK why this is needed. Seems like counter doesn't work properly without it*/
#define RJoinWordsB(x, y) RJoinWordsA(x, y)

/**Macro for registering a nullary/no-argument function to an app.*/
#define elkeRegisterNullaryFunction(func_name)                                 \
  static char RJoinWordsB(unique_var_name1_, __COUNTER__) =                    \
    elke::StaticRegister::registerNullaryFunction(#func_name, func_name)

#define elkeRegisterSyntaxBlock(class_name, block_syntax)                      \
  static char RJoinWordsB(unique_var_name2_, __COUNTER__) =                    \
    elke::StaticRegister::registerSyntaxBlock<class_name>(#class_name,         \
                                                          #block_syntax)

#define elkeRegisterObject(class_name)                                         \
  static char RJoinWordsB(unique_var_name2_, __COUNTER__) =                    \
    elke::StaticRegister::registerObject<class_name>(#class_name)

#define elkeRegisterNamedParameterTree(class_name)                                     \
  static char RJoinWordsB(unique_var_name2_, __COUNTER__) =                    \
    elke::StaticRegister::registerNamedParameterTree<class_name>(            \
      #class_name)

namespace elke
{

// ###################################################################
//  Helpers
using NullaryFunction = void (*)();

using GetParametersFunction = elke::ParameterTree (*)();

using SyntaxBlockPtr = std::shared_ptr<SyntaxBlock>;
using SyntaxBlockConstructionFunction =
  SyntaxBlockPtr (*)(const elke::ParameterTree&);

struct SyntaxBlockRegisterEntry
{
  std::string m_syntax;
  GetParametersFunction m_parameter_function = nullptr;
  SyntaxBlockConstructionFunction m_constructor_function = nullptr;
};

using FactoryObjectPtr = std::shared_ptr<FactoryObject>;
using FactoryObjectConstructionFunction =
  FactoryObjectPtr (*)(const elke::ParameterTree&);

struct FactoryObjectRegisterEntry
{
  GetParametersFunction m_parameter_function = nullptr;
  FactoryObjectConstructionFunction m_constructor_function = nullptr;
};

struct NamedParameterTreeRegistryEntry
{
  GetParametersFunction m_parameter_function = nullptr;
};

// ###################################################################
/**A singleton for static registration.*/
class StaticRegister
{
  std::map<std::string, NullaryFunction> m_nullary_function_register;
  std::map<std::string, SyntaxBlockRegisterEntry> m_syntax_block_register;
  std::map<std::string, FactoryObjectRegisterEntry> m_factory_object_register;
  std::map<std::string, NamedParameterTreeRegistryEntry>
    m_input_blocks_register;

public:
  static StaticRegister& getInstance();
  /**Returns the nullary functions registry.*/
  static const std::map<std::string, NullaryFunction>& getNullaryFunctions();
  /**Returns the syntax-block registry.*/
  static const std::map<std::string, SyntaxBlockRegisterEntry>&
  getSyntaxSystemRegister();
  /**Returns the factory-object register.*/
  static const std::map<std::string, FactoryObjectRegisterEntry>&
  getFactoryObjectRegister();
  /**Returns the input block register.*/
  static const std::map<std::string, NamedParameterTreeRegistryEntry>&
  getInputParameterBlockRegistry();

  static char registerNullaryFunction(const std::string& function_name,
                                      NullaryFunction function);

  template <typename TargetType>
  static char registerSyntaxBlock(const std::string& syntax_block_name,
                                  const std::string& syntax)
  {
    auto& registry = getInstance();

    SyntaxBlockRegisterEntry new_entry;

    new_entry.m_syntax = syntax;
    new_entry.m_parameter_function = TargetType::getInputParameters;
    new_entry.m_constructor_function =
      ProxySyntaxBlockConstructor<TargetType, SyntaxBlock>;

    registry.m_syntax_block_register[syntax_block_name] = new_entry;
    // TODO: Check for duplicates

    return 0;
  }

  template <typename TargetType>
  static char registerObject(const std::string& object_name)
  {
    auto& registry = getInstance();

    FactoryObjectRegisterEntry new_entry;

    new_entry.m_parameter_function = TargetType::getInputParameters;
    new_entry.m_constructor_function =
      ProxyFactoryObjectConstructor<TargetType, FactoryObject>;

    registry.m_factory_object_register[object_name] = new_entry;
    // TODO: Check for duplicates

    return 0;
  }

  template <typename TargetType>
  static char registerNamedParameterTree(const std::string& block_name)
  {
    auto& registry = getInstance();

    NamedParameterTreeRegistryEntry new_entry;

    new_entry.m_parameter_function = TargetType::getInputParameters;

    registry.m_input_blocks_register[block_name] = new_entry;

    return 0;
  }

  ///@{ Delete all copy/move/assignment methods.
  StaticRegister(StaticRegister const&) = delete;            // copy constructor
  StaticRegister(StaticRegister&&) = delete;                 // move constructor
  StaticRegister& operator=(StaticRegister const&) = delete; // assignment op
  StaticRegister& operator=(StaticRegister&&) = delete; // assignment move op
  ///@}

private:
  StaticRegister() = default;

  template <typename TargetType, typename BaseType>
  static std::shared_ptr<BaseType>
  ProxySyntaxBlockConstructor(const ParameterTree& params)
  {
    return std::make_shared<TargetType>(params);
  }
  template <typename TargetType, typename BaseType>
  static std::shared_ptr<BaseType>
  ProxyFactoryObjectConstructor(const ParameterTree& params)
  {
    return std::make_shared<TargetType>(params);
  }
};
} // namespace elke

#endif // ELKE_CORE_REGISTRATION_REGISTRATION_H
