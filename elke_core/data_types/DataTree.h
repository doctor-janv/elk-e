#ifndef ELK_E_DATATREE_H
#define ELK_E_DATATREE_H

#include "ScalarValue.h"
#include "DataGrossType.h"

#include <string>
#include <vector>
#include <functional>
#include <map>

/**What is a data tree? Well if you google JSON format... that is a data tree.
 *Requirements:
 * - Must be suitable to store a JSON/YAML/XML format style structure.
 * - Must be able to obtain data nodes via hierarchical reference of the form:
 *   'main_node/block1/subblock2/param_1'
 * - Values must be assignable via array.
 * - Each node is either a node (having 1 or more values) or a subtree
 */

namespace elke
{

/**Class to support a data tree2. The constructor options for this class is
 * super simple... there is only one choice. Create a DataTree by calling
 * the basic constructor
 * ```c++
 * auto root_tree = DataTree("NameOfTheTree");
 * ```
 * Once you have a tree you change or set several properties, the most important
 * of which are:
 * - `DataTree::rename`, which will change the name.
 * - `DataTree::setGrossType`, which will change its gross-type.
 * - `DataTree:setTag`, which will add a string name+value pair to the
 * tree.
 * - `DataTree::setValue`, which will assign a scalar value to the tree.
 * - `DataTree::addChild`, which will add a child tree.
 *
 * Reading from, or using, a DataTree is mostly done with the following methods:
 * - `DataTree::name`, which returns the name of the DataTree element.
 * - `DataTree::grossType`, which returns the gross-type.
 * - `DataTree::getTag`, which returns a named tag.
 * - `DataTree::value`, returns the scalar value if the tree has a
 *    gross-type of SCALAR. Implemented via `elke::ScalarValue`.
 * - `DataTree::child`, returns a child by name.
 * - `DataTree::children`, which returns an iterable list of all the
 *   tree's children.
 *
 * The Gross-Type:\n
 * Super simple, a DataTree can be any of the following:
 * - `DataGrossType::NO_DATA`, e.g. `scale: Null`, use this type
 *   sparingly
 * - `DataGrossType::SCALAR`, e.g. `scale: 1.2`
 * - `DataGrossType::SEQUENCE`, e.g. `scales: [1, 2, 3]`
 * - `DataGrossType::MAP`, e.g. `scales: {scalex: 1.1, scaley: 1.2}`
 *
 * Tags:\n
 * Tags are super useful for shuttling metadata from source files. Examples
 * - `DataTree::setTag("mark")` can be used to mark the file location (i.e.
 *   file-name, line-number and column-number).
 * - `DataTree::setTag("address")` can be used to find the address of a "leaf"
 *   within a hierarchy, e.g., `Input.yaml/systems/sub_object1/scale`. Useful
 *   for printing errors.
 */
class DataTree
{
  /**Function called during traversals.*/
  using DataTreeTraverseFunction =
    std::function<void(const std::string&, DataTree&)>;
  using DataTreePtr = std::shared_ptr<DataTree>;
  using DataTreeConstPtr = std::shared_ptr<const DataTree>;

  /// Name of the element
  std::string m_name;
  /// Gross-type
  DataGrossType m_gross_type = DataGrossType::NO_DATA;
  std::map<std::string, std::string> m_tags;
  ScalarValue m_value;
  std::vector<DataTreePtr> m_children;

public:
  /**Constructor requiring the name.*/
  explicit DataTree(std::string name);

  /**Copy constructor.*/
  DataTree(const DataTree& other)
    : m_name(other.m_name),
      m_gross_type(other.m_gross_type),
      m_tags(other.m_tags),
      m_value(other.m_value),
      m_children(other.m_children)
  {
  }

  /**Returns the name assigned to this tree.*/
  const std::string& name() const;

  /**Returns the general type of the data-tree.*/
  DataGrossType grossType() const;

  /**Sets the data-tree type.*/
  void setGrossType(DataGrossType type);

  /**Assigns a new name.*/
  void rename(const std::string& new_name);

  /**Returns a constant reference to the value.*/
  const ScalarValue& value() const;

  /**Adds a value to the node*/
  void setValue(const ScalarValue& value);

  /**Adds a child tree.*/
  void addChild(const DataTreePtr& child, bool prevent_duplicate = false);

  /**Set address tag*/
  void setTag(const std::string& tag_name, const std::string& tag_value);

  /**Gets a tag.*/
  std::string getTag(const std::string& tag_name) const;

  /**Traverses the tree and calls a callback function at each node.*/
  void traverseWithCallback(const std::string& running_address,
                            const DataTreeTraverseFunction& function,
                            const std::string& name_override = "");

  /**Returns a reference to a child of only the current level tree. If
   * the name is not found std::logic_error is thrown.*/
  DataTree& child(const std::string& child_name);

  /**Returns a const reference to a child of only the current level tree. If
   * the name is not found std::logic_error is thrown.*/
  const DataTree& child(const std::string& child_name) const;

  /**Determines if the data tree has the named child*/
  bool hasChild(const std::string& child_name) const;

  /**Returns the number of children.*/
  size_t numChildren() const { return m_children.size(); }

  /**Returns the list of children*/
  std::vector<DataTreePtr>& children();

  /**Returns the list of children*/
  std::vector<DataTreeConstPtr> constChildren() const;

  /**Makes a vector of all the children's gross-types.*/
  std::vector<DataGrossType> makeChildrenGrossTypesList() const;

  /**Produces a string in YAML format of the entire tree.*/
  std::string
  toStringAsYAML(const std::string& indent,
                 const std::vector<std::string>& tags_to_print = {}) const;
};

} // namespace elke

#endif // ELK_E_DATATREE_H
