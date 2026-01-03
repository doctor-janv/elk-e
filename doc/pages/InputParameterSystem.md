\page ParameterSystem The Parameter System

The `elke` input parameter system is quite complex as it uses the latest and
greatest of C++ template technology. It is debatable whether one should explain
this from a bottom-up approach or a top-down approach, however, for this page
we will use a top-down approach.

# 1 The high level objects
The two main players in the parameter system are:
- `elke::DataTree`, and
- `elke::InputParametersBlock`

## 1.1 The DataTree class
An `elke::DataTree` normally gets created from an input file like a json or 
yaml formatted file, although it can be manually created as well. The nature of 
the `elke::DataTree` uses an internal dependency to represent the branches of the
data-tree. In other words, an `elke::DataTree` can have connections to other
"child" data-trees also of the class type `elke::DataTree`.