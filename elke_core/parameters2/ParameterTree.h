#ifndef ELK_E_PARAMETERTREE_H
#define ELK_E_PARAMETERTREE_H

#include "ParameterTreeMetaData.h"
#include "param_checks.h"
#include "elke_core/data_types/DataGrossType.h"
#include "elke_core/data_types/ScalarValue.h"
#include "elke_core/utilities/general_utils.h"
#include "elke_core/utilities/special_iterators.h"
#include "ParameterTree_helpers.h"

#include <utility>
#include <iostream>

namespace elke
{

enum class ParameterLabel : int
{
  OPTIONAL = 0,
  REQUIRED = 1,
  DEPRECATED = 2
};

/**Generates the string equivalent of a parameter label.*/
std::string ParameterLabelToString(ParameterLabel label);

// ###################################################################
/**Object class to implement parameters. The initial design intent of the
 * ParameterTree is to loosely mimic the DataTree in the sense that both a
 * leaf and a branch is still a Tree, so it is a recurrence relation. With
 * a ParameterTree each level of assignment undergoes checking, i.e.,
 * - a scalar parameter requires that a compatible scalar be assigned to it.
 */
class ParameterTree
{
  using ParameterTreePtr = std::shared_ptr<ParameterTree>;
  using DataTreeConstPtr = std::shared_ptr<const DataTree>;

  const std::string m_name;         /// Name of the tree.
  const std::string m_description;  /// Description to provide queries.
  const ParameterLabel m_label;     /// Label, optional/required/deprecated
  const DataGrossType m_gross_type; /// See DataGrossType
  /// A pointer to a parent tree if applicable.
  const ParameterTree* m_parent_tree_ptr;
  /// Metadata used to control fine-grain checking and value assignment
  const ParameterTreeMetaData m_meta_data;
  /// If this tree is used a specification map, this value is true.
  /// Generally false unless it has been created with a constructor not
  /// needing a parent pointer.
  const bool m_is_a_specification_map = false;

  /// A list of all the children.
  std::vector<ParameterTreePtr> m_children;

  /// Additional input checks to run after generic tests have executed.
  std::vector<AdditionalInputCheckEntry> m_additional_input_checks;

  /// Raw assigned DataTree data.
  std::shared_ptr<const DataTree> m_assigned_data_tree = nullptr;

  // Runtime attributes BEGIN
  /**Nest depth. Set when called in recursive methods and a child has a
   * parent.*/
  size_t m_nest_depth = 0;

  /**Controls the skipping of assignment. If true, value assignment will
   * occur.*/
  bool m_assignment_flag = false;
  // Runtime attributes END

public:
  /**Deleted Default constructor.*/
  ParameterTree() = delete;

  /**Empty block creation. Can only be of map type.*/
  explicit ParameterTree(std::string name, std::string description);

  /**Generic constructor.*/
  explicit ParameterTree(const ParameterTree& parent,
                         std::string name,
                         std::string description,
                         const ParameterLabel label,
                         const DataGrossType gross_type,
                         ParameterTreeMetaData meta_data)
    : m_name(std::move(name)),
      m_description(std::move(description)),
      m_label(label),
      m_gross_type(gross_type),
      m_parent_tree_ptr(&parent),
      m_meta_data(std::move(meta_data))
  {
  }

  ParameterTree& operator+=(const ParameterTree& other);

  /**Returns the name of the tree.*/
  const std::string& name() const;
  /**Returns the label of the tree.*/
  ParameterLabel label() const;
  /**Sets the assignment flag for the particular tree.*/
  void setAssignmentFlag(bool value);

  // clang-format off
  /**Returns an iterable container for the parameters in this block.*/
  auto iterableParameters() { return VectorOfPointersIterable(m_children); }
  /**Returns a const-iterable container for the parameters in this block.*/
  auto constIterableParameters() const { return VectorOfPointersConstIterable(m_children);}
  // clang-format on

  /**Adds an optional parameter with default options.*/
  template <typename T>
  ParameterTree& addOptionalParameter(const std::string name,
                                      const std::string description,
                                      const T proxy_value)
  {
    return addParameter(name,
                        description,
                        ParameterLabel::OPTIONAL,
                        proxy_value,
                        ParameterTreeMetaData(),
                        /*custon_option=*/false);
  }

  /**Adds an optional parameter with custom options.*/
  template <typename T>
  ParameterTree& addOptionalParameter(const std::string name,
                                      const std::string description,
                                      const T proxy_value,
                                      const ParameterTreeMetaData options)
  {
    return addParameter(
      name, description, ParameterLabel::OPTIONAL, proxy_value, options);
  }

  /**Adds a required parameter with default options.*/
  template <typename T>
  ParameterTree& addRequiredParameter(const std::string name,
                                      const std::string description)
  {
    T proxy_value{};

    return addParameter(name,
                        description,
                        ParameterLabel::REQUIRED,
                        proxy_value,
                        ParameterTreeMetaData(),
                        /*custon_option=*/false);
  }

  /**Adds a required parameter with custom options.*/
  template <typename T>
  ParameterTree& addRequiredParameter(const std::string name,
                                      const std::string description,
                                      const ParameterTreeMetaData options)
  {
    T proxy_value{};
    return addParameter(
      name, description, ParameterLabel::REQUIRED, proxy_value, options);
  }

protected:
  /**Adds a Scalar parameter.*/
  template <typename T, std::enable_if_t<IsScalar<T>::value, bool> = true>
  ParameterTree& addParameter(const std::string& name,
                              const std::string& description,
                              const ParameterLabel label,
                              T proxy_value,
                              ParameterTreeMetaData options,
                              bool custom_options)
  {
    this->assertAndThrowIfDuplicate(name);

    if (not custom_options)
    {
      options.m_scalar_options.m_scalar_type = ScalarTypeEvaluation<T>::type;
      options.m_scalar_options.m_scalar_assignment =
        param_options::ScalarAssignment::MUST_BE_COMPATIBLE;
      options.m_scalar_options.m_default_value = ScalarValue(proxy_value);
    }

    const auto parameter_ptr = std::make_shared<ParameterTree>(
      *this, name, description, label, DataGrossType::SCALAR, options);

    m_children.emplace_back(parameter_ptr);

    return *parameter_ptr;
  }

  /**Adds a Vector parameter (array of scalar).*/
  template <typename T, std::enable_if_t<IsVector<T>::value, bool> = true>
  ParameterTree& addParameter(const std::string& name,
                              const std::string& description,
                              const ParameterLabel label,
                              T proxy_values,
                              ParameterTreeMetaData options,
                              bool custom_options)
  {
    using U = typename T::value_type;
    this->assertAndThrowIfDuplicate(name);

    if (not custom_options)
    {
      auto& array_options = options.m_array_options;
      array_options.m_nature = param_options::ArrayNature::SCALARS;

      auto& arr_of_scalars_options = array_options.m_array_of_scalar_options;
      arr_of_scalars_options.m_scalar_type = ScalarTypeEvaluation<U>::type;
      arr_of_scalars_options.m_scalar_assignment =
        param_options::ScalarAssignment::MUST_BE_COMPATIBLE;

      auto& default_values = arr_of_scalars_options.m_default_values;

      for (const auto& proxy_value : proxy_values)
        default_values.emplace_back(proxy_value);
    }

    const auto parameter_ptr = std::make_shared<ParameterTree>(
      *this, name, description, label, DataGrossType::SEQUENCE, options);

    m_children.emplace_back(parameter_ptr);

    return *parameter_ptr;
  }

  /**Adds a generic array parameter.*/
  ParameterTree& addParameter(const std::string& name,
                              const std::string& description,
                              const ParameterLabel label,
                              GenericParameterArray proxy_value,
                              ParameterTreeMetaData options,
                              const bool custom_options)
  {
    this->assertAndThrowIfDuplicate(name);

    if (not custom_options)
    {
      auto& array_options = options.m_array_options;
      array_options.m_nature = param_options::ArrayNature::ARBITRARY;

      auto& arr_of_arbs_options = array_options.m_array_of_arbs_options;

      auto& default_values = arr_of_arbs_options.m_default_values;

      for (const auto& value : proxy_value.defaultValues())
        default_values.emplace_back(value);
    }

    const auto parameter_ptr = std::make_shared<ParameterTree>(
      *this, name, description, label, DataGrossType::SEQUENCE, options);

    m_children.emplace_back(parameter_ptr);

    return *parameter_ptr;
  }

  /**Adds a generic map parameter.*/
  ParameterTree& addParameter(const std::string& name,
                              const std::string& description,
                              const ParameterLabel label,
                              GenericParameterMap proxy_value,
                              ParameterTreeMetaData options,
                              const bool custom_options)
  {
    this->assertAndThrowIfDuplicate(name);

    if (not custom_options)
    {
      auto& map_options = options.m_map_options;

      map_options.m_default_value = proxy_value.defaultValue();
    }

    const auto parameter_ptr = std::make_shared<ParameterTree>(
      *this, name, description, label, DataGrossType::MAP, options);

    m_children.emplace_back(parameter_ptr);

    return *parameter_ptr;
  }

public:
  /**Adds additional input checks. If used with an existing pointer, use
   *std::move, otherwise use the curly braces with std::make_unique.
   *
   *\param check_ptr A unique pointer to the new check object.
   *\param allow_subsequent_checks Default true. If true, then checks after this
   *one will execute even if this check failed.
   *Example:
   *
   * \code
   * auto params = ParameterTree("NoName", "Description");
   *
   * auto new_ptr = std::make_unique<param_checks::ArraySizeCheck>(3);
   * param.addAdditionalInputCheck(std::move(new_ptr));
   * // or
   * param.addAdditionalInputCheck({std::make_unique<param_checks::ArraySizeCheck>(3)});
   * \endcode
   */
  void
  addAdditionalInputCheck(std::unique_ptr<param_checks::InputCheck> check_ptr,
                          bool allow_subsequent_checks = true);

  /**Perform all additional checks.*/
  bool performAdditionalChecks(StatusStrings& status_strings,
                               const DataTree& data,
                               const std::string& prefix,
                               const std::string& name_or_id) const;

  /**Checks whether the parameter with the given name is present.*/
  bool hasParameter(const std::string& name) const;

  /**Obtains const-reference to a parameter by name.*/
  const ParameterTree& getParameter(const std::string& name) const;

  /**Obtains a non-const-reference to a parameter by name.*/
  ParameterTree& getParameter(const std::string& name);

  /**Entry point for checking a parameter tree. This routines will check the
   * gross-type is correct then specialize into specific types.
   * \param status_strings A reference to a StatusString data-structure where
   *                       error/warnings messages can be dumped.
   * \param data A reference to the data-tree item that is to be assigned to the
   *             parameter.
   */
  void checkAndAssignData(StatusStrings& status_strings, const DataTree& data);

  void checkAndAssignScalarData(StatusStrings& status_strings,
                                const DataTree& data);

  void checkAndAssignSequenceData(StatusStrings& status_strings,
                                  const DataTree& data);
  void checkAndAssignArrayOfScalars(StatusStrings& status_strings,
                                    const DataTree& data);
  void checkAndAssignArrayOfArbs(StatusStrings& status_strings,
                                 const DataTree& data);
  void checkAndAssignArbitraryMap(StatusStrings& status_strings,
                                  const DataTree& data);

  void processSpecification(StatusStrings& output_status_strings,
                            const DataTree& data);

private:
  /**Creates a sorted list of all the children names.*/
  std::vector<std::string> validParameterNames() const;
  /**If the new parameter name is already in the list of children, a
   * std::logic_error is thrown. */
  void assertAndThrowIfDuplicate(const std::string& new_parameter_name) const;

  /**Checks if gross type matches.*/
  bool grossTypeMatches(std::string& error_string, const DataTree& data) const;

  /**Get assignment flag. This is recursive on the parent of this tree.*/
  bool getRecursiveParentAssignmentFlag() const;

  /**Recursively determines the current nesting depth.*/
  size_t getRecursiveNestDepth() const;

  /**Assigns a parent.*/
  void setParent(const ParameterTree* const parent)
  {
    m_parent_tree_ptr = parent;
  }
}; // class ParameterTree

} // namespace elke

#endif // ELK_E_PARAMETERTREE_H
