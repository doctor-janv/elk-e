#ifndef ELK_E_PARAMETERCHECK_H
#define ELK_E_PARAMETERCHECK_H
#include "elke_core/data_types/DataTree.h"

namespace elke
{

class InputParameter;
class DataTree;

} // namespace elke

struct ParameterCheckResult
{
  std::string m_error;
  std::string m_warning;
};

namespace elke::param_checks
{

// ###################################################################
/**Base class for a parameter check.*/
class ParameterCheck
{
protected:
  ParameterCheck() = default;

public:
  virtual ~ParameterCheck() = default;
  virtual ParameterCheckResult checkParameter(const DataTree& data,
                                              const InputParameter& parameter,
                                              unsigned int nest_depth) = 0;
};

// ###################################################################
/**A strict type check on the gross type.*/
class GrossTypeMustMatch final : public ParameterCheck
{
public:
  ParameterCheckResult checkParameter(const DataTree& data,
                                      const InputParameter& parameter,
                                      unsigned int nest_depth) override;
};

// ###################################################################
/**A strict type check on the scalar type.*/
class ScalarTypeMustMatch final : public ParameterCheck
{
  GrossTypeMustMatch m_gross_check;

public:
  ParameterCheckResult checkParameter(const DataTree& data,
                                      const InputParameter& parameter,
                                      unsigned int nest_depth) override;
};

// ###################################################################
/**A loose type check on the scalar type.*/
class ScalarTypeMustBeCompatible final : public ParameterCheck
{
  GrossTypeMustMatch m_gross_check;

public:
  ParameterCheckResult checkParameter(const DataTree& data,
                                      const InputParameter& parameter,
                                      unsigned int nest_depth) override;
};

// ###################################################################
class ScalarArrayEntriesTypeMustMatch final : public ParameterCheck
{
public:
  ParameterCheckResult checkParameter(const DataTree& data,
                                      const InputParameter& parameter,
                                      unsigned int nest_depth) override;
};

// ###################################################################
class ScalarArrayEntriesTypeMustBeCompatible final : public ParameterCheck
{
public:
  ParameterCheckResult checkParameter(const DataTree& data,
                                      const InputParameter& parameter,
                                      unsigned int nest_depth) override;
};

// ###################################################################
class RegisteredObjectMustExist final : public ParameterCheck
{
public:
  ParameterCheckResult checkParameter(const DataTree& data,
                                      const InputParameter&,
                                      unsigned int nest_depth) override;
};

// ###################################################################
class RegisteredObjectArrayEntriesMustExist final : public ParameterCheck
{
public:
  ParameterCheckResult checkParameter(const DataTree& data,
                                      const InputParameter& parameter,
                                      unsigned int nest_depth) override;
};

} // namespace elke::param_checks

#endif // ELK_E_PARAMETERCHECK_H
