#ifndef ELK_E_INPUTPROCESSOR_H
#define ELK_E_INPUTPROCESSOR_H
#include "elke_core/data_types/DataTree.h"

#include <string>

namespace elke
{

/**Base class for input processors.*/
class InputProcessor
{
public:
  /**Reads an input file and produces a DataTree.*/
  virtual elke::DataTree readInputFile(std::string file_name) = 0;
protected:
  InputProcessor() = default;
  virtual ~InputProcessor() = default;
};

} // namespace elke

#endif // ELK_E_INPUTPROCESSOR_H
