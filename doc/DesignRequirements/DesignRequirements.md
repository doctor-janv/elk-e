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
as a dynamic library. Program instances shall commence with a `CoreModule`, which 
shall be a singleton within each process, via an `execution` static method.

<!--endreq-->

## Requirement input_processing - There shall be a distinct input processing phase
During this phase all possible input processing
shall be performed and all errors shall be collected to be displayed/conveyed at the end of the
phase.
<!--endreq-->

## Requirement cli1 - Command Line Interface

Elke based programs shall support command line arguments (CLAs) in the following form
- Parameter value pairs. I.e., `-p value` and `--param value` where `p` is the
  shorthand version of `param` and is interchangeable with any other.
- Items marked as `value` can start with a `-` or a `--` as long as it does not
  conflict with other command line arguments in the application.
- A CLA does not have to have a value, and can thus be used as a flag.
- Some CLAs could be used multiple times. Functionality should exist to allow for it.
- The `-h` flag summons help

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

## Requirement blocks - Input files have syntax-blocks
- Input files shall be processed via syntax-blocks.
- There shall be a "Simulation" syntax block that serves as an entry point for
  general settings (such as loading plugins).

<!--endreq-->

## Requirement inparams1 - Object instantiation via input parameters

### Definition of an input parameter
An input parameter can be one of following:
- A string
- A number
- An array of numbers
- An array of strings

<!--endreq-->

## Requirement friendly_errors - The code should output friendly errors
- When a file is not found a friendly error should be printed at least showing
  which file could not be found.
