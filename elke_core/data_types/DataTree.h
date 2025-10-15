#ifndef ELK_E_DATATREE_H
#define ELK_E_DATATREE_H

#include "Varying.h"

#include <string>
#include <vector>
#include <functional>

/**What is a data tree? Well if you google JSON format... that is a data tree.
 *Requirements:
 * - Must be suitable to store a JSON/YAML/XML format style structure.
 * - Must be able to obtain data nodes via hierarchical reference of the form:
 *   'main_node/block1/subblock2/param_1'
 * - Values must be assignable via array.
 * - Each node is either a node (having 1 or more values) or a sub-tree
 */

namespace elke
{

class DataTree
{
  /**Function called during traversals.*/
  using DataTreeTraverseFunction =
    std::function<void(const std::string&, DataTree&)>;
  using DataTreePtr = std::shared_ptr<DataTree>;
  std::string m_name;
  std::vector<Varying> m_values;
  std::vector<DataTreePtr> m_children;

public:
  /**Constructor requiring the name.*/
  explicit DataTree(std::string name);

  /**Returns the name assigned to this tree.*/
  const std::string& name() const;

  /**Returns a constant reference to the values.*/
  const std::vector<Varying>& values() const;

  /**Returns a reference to the values.*/
  std::vector<Varying>& values();

  /**Traverses the tree and calls a callback function at each node.*/
  void traverseWithCallback(const std::string& running_address,
                            const DataTreeTraverseFunction& function);

  /**Adds a child tree.*/
  void addChild(const DataTreePtr& child);

  /**Produces a string in YAML format of the entire tree.*/
  std::string toStringAsYAML(const std::string& indent = "") const;

  // private:
  //   void addValues(const std::vector<Varying>& values);
  //   void addChild(DataTreePtr child);
};

} // namespace elke

#endif // ELK_E_DATATREE_H
