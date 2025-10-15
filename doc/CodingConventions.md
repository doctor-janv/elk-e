# Coding conventions

## 1 Declarations and Implementations
  - As far as possible, **header files** (.h) should contain declarations and **source files** (.cc)
    should contain definitions/implementations.
  - Templated class/routines/methods are allowed to be header only. If they have specific
    implementations for specific types then those should be within the source file.

## 2 Naming

In general, there are the following naming conventions:
  - lowercase: `myvariable`
  - camelCase: `myVariable`
  - PascalCase: `MyVariable`
  - snake_case: `my_variable`
  - Leading_upper_snake_case: `My_Variable`
  - Upper_Snake_Case: `MY_VARIABLE`
  - SCREAMING_SNAKE_CASE: `MY_VARIABLE`
  - UPPERCASE: `MYVARIABLE `

### 2.1 Naming Classes and Structs
Classes and Structs are to use `PascalCase`, i.e.,

```
class CommandLineArgument
struct MetaData
```

### 2.2 Naming subroutines, functions, and class/struct methods
These are all `camelCase`. Under rare circumstances this may be bent a little, e.g. 
`computeCHF_1, computeCHF_2`.


### 2.3 Naming files and folders
Folder names are snake_case, e.g., `elke_core`, `cli`, `data_types`, etc.

- **Header files**. If a header file contains the declaration of an object then
  the filename should match the object. E.g, `class SetupOptions` should be
  named `SetupOptions.h`. If the header file contains a bunch of utilities, use
  snakecase, i.e. `c_api.h` or `registration.h`.
- **Source files**
  - Class/Struct definitions. Mixed case allowed. Different situations.
    - Only one definition file. `Object.h` and `Object.cc`.
    - Multiple definition files. May contain numerical + categorical
      annotations, e.g., `Object_01_constrdestr.cc` and `Object_02_init.cc`
  - routine/functions/methods definitions. `pascalCase`. Can contain alphabetizing
    prefixes with an underscore, e.g., `setupGlobalCLI.cc`,
    `a01_setupGlobalCLI.cc`

## 3 Class structure
C++ object classes follow the convention given by Robert C. Martin in his book "Clean Code".
Public members should be on top, followed by private/protected members, to make it easier to 
identify what this object consists of. The methods do not need to follow a particular public-first
ordering (although it makes it easier to read). Just focus on telling the class's story.
```c++
class ClassName
{
public:
    // Members
private:
    // Members
//Methods
    //tell a story, dont worry to mix public vs private
};
```

## 4 Namespaces
Namespaces should be snakecase and not used beyond `elke` unnecessarily. Namespaces should always
be closed with a comment like this:
```c++
namespace elke
{
//Stuff
} // namespace elke
```

## 5 Readibility comments
- To clearly distinguish the start of routines/functions/methods one may use `//########...###` up to
  column 70.
  ```c++
  //###################################################################
  /**Traverses the tree and calls a callback function at each node.*/
  void DataTree::traverseWithCallback(const std::string& running_address,
                                      const DataTreeTraverseFunction& function)
  {
    const std::string current_address =
      running_address.empty() ? m_name + "/" : running_address + m_name + "/";
  
    function(current_address, *this);
  
    for (const auto& child : m_children)
      child->traverseWithCallback(current_address, function);
  }
  
  //###################################################################
  /**Adds a child tree.*/
  void DataTree::addChild(const DataTreePtr& child)
  {
    m_children.push_back(child);
  }
  ```
- Sections within a routine and subsections. Cleverly use `//======...====` to column with multiples
  of 10. For example:
  ```c++
  std::string DataTree::toStringAsYAML(const std::string& indent /*=""*/) const
  {
    std::stringstream yaml;
  
    yaml << indent << m_name << ":";
  
    //=================================== Branch case this is NOT a subtree
    if (m_children.empty())
    {
      //============================ Single value
      if (m_values.size() == 1)
      {
        yaml << " " << m_values.front().PrintStr(false) << "\n";
      }
      //============================ Array value
      else
      {
        yaml << "[";
        for (const auto& value : m_values)
        {
          yaml << value.PrintStr(/*with_type=*/false);
          if (value != *m_values.rbegin()) yaml << ", ";
        }
        yaml << "]\n";
      }
    } // m_children.empty()
    //=================================== Branch case: this is a subtree
    else
    {
      yaml << "\n";
      for (const auto& child : m_children)
        yaml << child->toStringAsYAML(indent + std::string(4, ' '));
    }
  
    return yaml.str();
  }
  ```
- Default value comments in implementations. If the declaration specifies a defualt value, for example
  in the header file you might have `PrintStr(const bool with_type=false) const`, then in the 
  definition or implementation, use this: `PrintStr(const bool with_type /*=false*/) const`.
- Try and close code blocks with a comments, e.g.
  ```c++
  if (zorba < 1)
  {
    //A lot of stuff
  } // zorba < 1
  
  for (int i=0; i<N_i; ++i)
  {
    for (int j=0; j<N_j; ++j)
    {
        //Stuff
    }//for j 
  }//for i
  ```