# Coding conventions

## 1 Naming

In general, there are the following naming conventions:
  - lowercase: `myvariable`
  - camelCase: `myVariable`
  - PascalCase: `MyVariable`
  - snake_case: `my_variable`
  - Leading_upper_snake_case: `My_Variable`
  - Upper_Snake_Case: `MY_VARIABLE`
  - SCREAMING_SNAKE_CASE: `MY_VARIABLE`
  - UPPERCASE: `MYVARIABLE `

### 1.1 Naming Classes and Structs
Classes and Structs are to use `PascalCase`, i.e.,

```
class CommandLineArgument
struct MetaData
```

### 1.2 Naming subroutines, functions, and class/struct methods
These are all camelCase. Under rare circumstances this may be bent a little.


### 1.3 Naming files and folders
Folder names are snake_case, e.g., `elke_core`, `cli`, `data_types`, etc.

- **Header files**. If a header file contains the declaration of an object then
  the filename should match the object. E.g, `class SetupOptions` should be
  named `SetupOptions.h`
- **Source files**
  - Class/Struct definitions. Mixed case allowed. Different situations.
    - Only one definition file. `Object.h` and `Object.cc`.
    - Multiple definition files. May contain numerical + categorical
      annotations, e.g., `Object_01_constrdestr.cc` and `Object_02_init.cc`
  - Subroutine/function definitions. snake_case. Can contain alphabetizing
    prefixes with an underscore, e.g., `setupGlobalCLI.cc`,
    `a01_setupGlobalCLI.cc`


