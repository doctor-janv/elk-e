#include "DataTree.h"

#include <utility>
#include <sstream>

namespace elke
{
/**Constructor requiring the name.*/
DataTree::DataTree(std::string name) : m_name(std::move(name)) {}

/**Returns the name assigned to this tree.*/
const std::string& DataTree::name() const { return m_name; }

/**Returns a constant reference to the values.*/
const std::vector<Varying>& DataTree::values() const { return m_values; }

/**Returns a reference to the values.*/
std::vector<Varying>& DataTree::values() { return m_values; }

//###################################################################
/**Traverses the tree and calls a callback function at each node.*/
void DataTree::traverseWithCallback(const std::string& running_address,
                                    const DataTreeTraverseFunction& function)
{
  const std::string current_address =
    running_address.empty() ? m_name + "/" : running_address + m_name + "/";

  function(current_address, *this);

  for (const auto& child : m_children)
    child->traverseWithCallback(current_address, function);
}

//###################################################################
/**Adds a child tree.*/
void DataTree::addChild(const DataTreePtr& child)
{
  m_children.push_back(child);
}

//###################################################################
/**Produces a string in YAML format of the entire tree.*/
/* Extreme use case. A python dictionary like
 *
MainInput = dict(
    component_100 = dict(
        type = "snglvol",
        xgeometry = dict(area = 0.25, length = 1.0),
        initial_conditions = dict(pressure = 1.0e6, temperature = 300.0),
        param1 = [2,3,4],
        param2 = ["abc", "def"],
        param3 = ["mixed", 123, 45.6]
    )
)

Should be visualized as
MainInput:
    component_100:
        type: "snglvol"
        xgeometry:
            area: 0.25
            length: 1
        initial_conditions:
            pressure: 1e+06
            temperature: 300
        param1:[2, 3, 4]
        param2:["abc", "def"]
        param3:["mixed", 123, 45.6]
*/
std::string DataTree::toStringAsYAML(const std::string& indent /*=""*/) const
{
  std::stringstream yaml;

  yaml << indent << m_name << ":";

  //=================================== Branch case this is NOT a subtree
  if (m_children.empty())
  {
    //============================ Single value
    if (m_values.size() == 1)
    {
      yaml << " " << m_values.front().PrintStr(false) << "\n";
    }
    //============================ Array value
    else
    {
      yaml << "[";
      for (const auto& value : m_values)
      {
        yaml << value.PrintStr(/*with_type=*/false);
        if (value != *m_values.rbegin()) yaml << ", ";
      }
      yaml << "]\n";
    }
  } // m_children.empty()
  //=================================== Branch case: this is a subtree
  else
  {
    yaml << "\n";
    for (const auto& child : m_children)
      yaml << child->toStringAsYAML(indent + std::string(4, ' '));
  }

  return yaml.str();
}

} // namespace elke