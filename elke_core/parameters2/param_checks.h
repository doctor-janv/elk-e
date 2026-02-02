#ifndef ELK_E_PARAM_CHECKS_H
#define ELK_E_PARAM_CHECKS_H

#include "elke_core/utilities/general_utils.h"
#include "elke_core/data_types/DataTree.h"
#include "elke_core/utilities/template_helpers.h"

namespace elke::param_checks
{

/**Base class helper object to define custom checking routines.*/
class InputCheck
{
  InputCheck() = default;
  template <typename T>
  friend class InputCheckCloneAble;
public:
  virtual ~InputCheck() = default;

  virtual std::unique_ptr<InputCheck> clone() const = 0;
  virtual bool performCheck(StatusStrings& status_strings,
                            const DataTree& data,
                            const std::string& prefix,
                            const std::string& name_or_id) const = 0;
};

/**Intermediate base class to connect the CRTP to the polymorphic
 *interface. */
template <typename Derived>
class InputCheckCloneAble : public InputCheck, public Clonable<Derived>
{
public:
  std::unique_ptr<InputCheck> clone() const override
  {
    // Call the CRTP clone method and wrap the result in unique_ptr
    return std::unique_ptr<InputCheck>(Clonable<Derived>::clone_helper());
  }
};

/**Check to enforce the number of children.*/
class ArraySizeCheck final : public InputCheckCloneAble<ArraySizeCheck>
{
  size_t m_size;

public:
  explicit ArraySizeCheck(const size_t size) : m_size(size) {}

  bool performCheck(StatusStrings& status_strings,
                    const DataTree& data,
                    const std::string& prefix,
                    const std::string& name_or_id) const override;
};

} // namespace elke::param_checks

#endif // ELK_E_PARAM_CHECKS_H
