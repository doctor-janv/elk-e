#ifndef ELK_E_DATATREE_H
#define ELK_E_DATATREE_H

#include "Varying.h"

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

enum class DataTreeType : int
{
  NO_DATA = 0,  ///< An unpopulated data-tree
  SCALAR = 1,   ///< Contains a single value, no children
  SEQUENCE = 2, ///< Contains multiple values, no children
  MAP = 3       ///< Contains no values, multiple children
};

/**Returns a string representation of the type.*/
std::string DataTreeTypeName(DataTreeType type);

/**Class to support a data tree.*/
class DataTree
{
  /**Function called during traversals.*/
  using DataTreeTraverseFunction =
    std::function<void(const std::string&, DataTree&)>;
  using DataTreePtr = std::shared_ptr<DataTree>;
  using DataTreeConstPtr = std::shared_ptr<const DataTree>;

  std::string m_name;
  DataTreeType m_type = DataTreeType::NO_DATA;
  std::map<std::string, std::string> m_tags;
  Varying m_value;
  std::vector<DataTreePtr> m_children;

public:
  /**Constructor requiring the name.*/
  explicit DataTree(std::string name);

  /**Returns the name assigned to this tree.*/
  const std::string& name() const;

  /**Returns the general type of the data-tree.*/
  DataTreeType type() const;

  /**Sets the data-tree type.*/
  void setType(DataTreeType type);

  /**Assigns a new name.*/
  void rename(const std::string& new_name);

  /**Returns a constant reference to the values.*/
  const Varying& value() const;

  /**Adds a value to the node*/
  void setValue(const Varying& value);

  /**Adds a child tree.*/
  void addChild(const DataTreePtr& child);

  /**Set address tag*/
  void setTag(const std::string& tag_name, const std::string& tag_value);

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

  /**Returns the number of children.*/
  size_t numChildren() const { return m_children.size(); }

  // /**Returns a const reference to a child at any level of the tree. If
  //  * the name is not found std::logic_error is thrown.*/
  // const DataTree& childByAddress(const std::string& address) const;

  /**Produces a string in YAML format of the entire tree.*/
  std::string
  toStringAsYAML(const std::string& indent,
                 const std::vector<std::string>& tags_to_print = {}) const;
};

} // namespace elke

#endif // ELK_E_DATATREE_H
