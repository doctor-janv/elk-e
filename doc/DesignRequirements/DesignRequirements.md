# ELK-E Design Requirements

## Requirement - Documented Coding conventions

There shall be a documented set of coding conventions.

## Requirement 1 - Command Line Interface

Elke based programs shall support command line arguments in the following form
- Parameter value pairs. I.e., `-p value` and `--param value` where `p` is the
  short-hand version of `param` and is interchangeable with any other.
- Expressions. `--p=expression`

Items marked as `value` can start with a `-` or a `--` as long as it does not
conflict with other command line arguments in the application.


## Requirement 2 - Object instantiation via input parameters

### Definition of an input parameter
An input parameter can be one of following:
- A string
- A number
- An array of numbers
- An array of strings
