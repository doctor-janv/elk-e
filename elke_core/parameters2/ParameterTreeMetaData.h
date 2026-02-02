#ifndef ELK_E_PARAMETERTREEOPTIONS_H
#define ELK_E_PARAMETERTREEOPTIONS_H

#include "elke_core/data_types/ScalarValue.h"
#include "elke_core/data_types/DataTree.h"

#include <cstdint>

namespace elke::param_options
{

// clang-format off
//################################################################### SCALAR
enum class ScalarAssignment : uint32_t
{
  MUST_MATCH_EXACTLY = 0,
  MUST_BE_COMPATIBLE = 1,
  CAN_BE_ANYTHING = 2
};

struct ScalarOptions
{
  ScalarType m_scalar_type = ScalarType::VOID;
  ScalarAssignment m_scalar_assignment = ScalarAssignment::MUST_BE_COMPATIBLE;
  ScalarValue m_default_value;
};

//################################################################### ARRAY
enum class ArrayNature : uint32_t
{
  SCALARS = 1,
  ARBITRARY = 4
};

//============================================= ARRAY OF SCALARS
struct ArrayOfScalarsOptions
{
  ScalarType m_scalar_type = ScalarType::VOID;
  ScalarAssignment m_scalar_assignment = ScalarAssignment::MUST_BE_COMPATIBLE;
  std::vector<ScalarValue> m_default_values;
};

//============================================= ARRAY OF ARBS
struct ArrayOfArbitraryOptions
{
  std::vector<DataTree> m_default_values;
};

struct ArrayOptions
{
  ArrayNature m_nature = ArrayNature::ARBITRARY;
  ArrayOfScalarsOptions m_array_of_scalar_options;
  ArrayOfArbitraryOptions m_array_of_arbs_options;
};

//################################################################### ARRAY
struct MapOptions
{
  DataTree m_default_value;
  MapOptions() : m_default_value("Default") {}
};

// clang-format on
} // namespace elke::param_options

namespace elke
{

struct ParameterTreeMetaData
{
  param_options::ScalarOptions m_scalar_options;
  param_options::ArrayOptions m_array_options;
  param_options::MapOptions m_map_options;
};

} // namespace elke

#endif // ELK_E_PARAMETERTREEOPTIONS_H
