# ELK-E Design Requirements
\page DesignRequirements Design Requirements

## Convention in this document

- We will use a parser to build a requirement database therefore a few keywords
  are reserved.
- Using a double hash and the word `Requirement`, i.e., `## Requirement`, at the
  beginning of a line (the hashes must start at column 0) triggers the third
  word as the requirement text name. For example:
  `## Requirement cli1` denotes requirement `cli1` follows.
- The block of requirements is terminated with `<!--endreq-->` with the apostrophes.

## Requirement basic1 - Basic program philosophy
There shall be the ability to instantiate the program via an executable as well
as a dynamic library. Program instances shall commence with a core module, which 
shall be a singleton within each process, via an `execution` static method. All 
processes must have access to the core-module.

<!--endreq-->

## Requirement cli1 - Command Line Interface

Elke based programs shall support command line arguments (CLAs) in the following form
- a. Parameter value pairs. I.e., `-p value` and `--param value` where `p` is the
  shorthand version of `param` and is interchangeable with any other.
- b. Items marked as `value` can start with a `-` or a `--` as long as it does not
  conflict with other command line arguments in the application. E.g., if the 
  registed CLAs are `-n`/`--name` and `-l`/`--last-name` then the following 
  would be illegal: `-n -l` (must trigger error requiring a "value" for `-n`).
- c. The following standard CLAs should be supported
  - `-h`/`--help`, with no value should print the Command Line Interface.
  - `--nocolor`, with no value should turn off all color printing.
  - `-v`/`--verbosity`, with an integer value should change the logger's 
    verbosity.
- c. A CLA does not necessarily have to have a value, and can thus be used as a flag.
- d. Some CLAs could be used multiple times. Functionality should exist to allow for it.

<!--endreq-->

## Requirement utesting - Unit testing
- As a CLA the user can use `basic` to execute unit-testing.

<!--endreq-->


## Requirement input_style - Input-style shall follow a data-tree paradigm
```yaml
main_tree:
  null_value: null             # A special node value
  scalar_value1: "A string!"   # A string
  scalar_value2: true          # A boolean
  scalar_value3: 123           # An integer
  scalar_value4: 123.4         # A floating point value
  a_sequence:
    - 123     # A child integer
    -         # A child sub-tree
      name: "Zorba"
      value: 12.0
  a_map:
    value: 123
    sub_tree:
      name: "Zorba"
      value: 12.0
```
- A data-tree can be one of four types: a `scalar`, a `sequence` (or array),
  a `map`, or the special empty type `null`. 
  - A `scalar` is a tree-node with no children, but a single value, where the value
    refers to one of the primitive types, i.e., `string`, `boolean`, `integer`,
    `float` or `null`.
  - A `sequence` is a tree-node where there is no value but rather a list of 
    unnamed children in a sequential order, akin to Python's `list`.
  - A `map` is a tree-node where there is no value but rather a list of named
    children, akin to Python's `map`.
    
- The internal data structures should support a variety of input file types, 
  e.g., `yaml`, `json`, `xml`, etc. capable of building a data-tree
- The data-tree structure should be furnished with tags, including:
  - `type`-tag, to easily observe its type for testing
  - `address`-tag, used to identify its position in a master-tree which will
    be used for error reporting
  - `mark`-tag, used to indicate the origin file, line- and column number. Also
    used for error reporting.
- No duplicates can be allowed (standard YAML)

<!--endreq-->

## Requirement input_parsing_phase - There shall be a distinct input parsing phase
During this phase all possible input files shall be parsed to data trees.

<!--endreq-->

## Requirement input_checking_phase - There shall be a distinct input checking phase
During this phase all possible input processing
shall be performed and all errors shall be collected to be displayed/conveyed at the end of the
phase.
<!--endreq-->

## Requirement blocks - Input files have syntax-blocks
- Input files shall be processed via syntax-blocks.
- There shall be a "Simulation" syntax block that serves as an entry point for
  general settings (such as loading plugins).

<!--endreq-->

## Requirement inparams1 - Object instantiation via input parameters

Definitions:
- Factory-object: An object created via a factory pattern

Requirements:
- A factory-object shall have a constructor that takes, as an argument, an 
  `InputParametersBlock` class object.
- A factory-object shall have a static method to build the basis of the 
  `InputParametersBlock` it requires. I.e., the signature shall be
  `static InputParametersBlock getInputParameters();`
- An `InputParametersBlock` shall always have a name and a description.
- An `InputParametersBlock` can inherit the input parameters of several
  other blocks.

- An input parameter shall always have a name.
- An input parameter shall be assigned a class-type which can be:
  - OPTIONAL
  - REQUIRED
  - DEPRECATED
- An input parameter shall be assigned a gross-type which can be:
  - NO_DATA, signifying it holds no data
  - SCALAR, holding a single value
  - SEQUENCE, holding multiple un-named children
  - MAP, holding multiple named children
- For scalar parameters, a scalar-type shall be assigned which can be:
  - STRING
  - BOOL
  - INTEGER
  - FLOAT
- Scalar assignment for non-matching scalars shall be possible, with appropriate 
  controls. In other words, by default if one scalar-type "can" be assigned to
  another it should be allowed. If the developer wants to enforce strict typing,
  it should also be possible to do so.
  
### Definition of an input parameter
- An input parameter can be one of following:
  - A string
  - A number (floating-point or integer)
  - A boolean
  - An array of numbers (floats and/or integer)
  - An array of strings
  - An array of booleans
  - An array of input-blocks

### Properties
- An input parameter always has a name.

- OPTIONAL parameters shall always have a default value.
- 

<!--endreq-->

## Requirement invalid_parameters - Invalid parameter names
If the input data contains a named data item that is NOT in the corresponding
input parameters, an error shall be generated.

<!--endreq-->

## Requirement friendly_errors - The code should output friendly errors
- When a file is not found a friendly error should be printed at least showing
  which file could not be found.
- When an input parameter is slighly mispelled, the code should provide a
  "Did you mean..." type message providing a suggestion.

<!--endreq-->

## Requirement friendly_runtime_errors - Developers need a means to generate runtime errors

A routine should be callable to post an error message with a stack-trace.

<!--endreq-->


