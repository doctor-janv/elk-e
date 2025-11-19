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





## Requirement inparams1 - Object instantiation via input parameters

### Definition of an input parameter
An input parameter can be one of following:
- A string
- A number
- An array of numbers
- An array of strings

<!--endreq-->
