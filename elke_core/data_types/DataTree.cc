#include "DataTree.h"

#include <utility>
#include <sstream>

namespace elke
{
/**Constructor requiring the name.*/
DataTree::DataTree(std::string name) : m_name(std::move(name)) {}

/**Returns the general type of the data-tree.*/
DataTreeType DataTree::type() const
{
  return m_type;
}

/**Sets the data-tree type.*/
void DataTree::setType(const DataTreeType type)
{
  m_type = type;
}

/**Returns the name assigned to this tree.*/
const std::string& DataTree::name() const { return m_name; }

/**Assigns a new name.*/
void DataTree::rename(const std::string& new_name) { m_name = new_name; }

/**Returns a constant reference to the values.*/
const Varying& DataTree::value() const { return m_value; }

/**Adds a value to the node*/
void DataTree::setValue(const Varying& value)
{
  if (m_type != DataTreeType::SCALAR)
    throw std::runtime_error(
      "Attempting to add value to DataTree " + m_name +
      " which has not been designated as a DataTreeType::Scalar");
  m_value = value;
}

// ###################################################################
/**Adds a child tree.*/
void DataTree::addChild(const DataTreePtr& child)
{
  if (not (m_type == DataTreeType::SEQUENCE or m_type == DataTreeType::MAP))
    throw std::runtime_error(
      "Attempting to add child to DataTree " + m_name +
      " which is not designated as either a SEQUENCE or a MAP.");
  m_children.push_back(child);
}

// ###################################################################
/**Traverses the tree and calls a callback function at each node.*/
void DataTree::traverseWithCallback(const std::string& running_address,
                                    const DataTreeTraverseFunction& function,
                                    const std::string& name_override /* = ""*/)
{
  const std::string name = name_override.empty() ? m_name : name_override;
  const std::string current_address = running_address + name + "/";

  function(current_address, *this);

  if (m_type == DataTreeType::SEQUENCE)
  {
    size_t id = 0;
    for (const auto& child : m_children)
    {
      child->traverseWithCallback(current_address, function, std::to_string(id));
      ++id;
    }
  }
  else if (m_type == DataTreeType::MAP)
    for (const auto& child : m_children)
      child->traverseWithCallback(current_address, function);
}



// ###################################################################
/**Produces a string in YAML format of the entire tree.
Extreme use case. A python dictionary like

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
  // Every parent is responsible for indenting its own children.
  const auto child_indent = indent + std::string(2, ' ');

  std::stringstream yaml;

  const std::string node_title = m_name.empty() ? "" : m_name + ":";

  yaml << node_title;

  switch (this->type())
  {
    case DataTreeType::NO_DATA:
      yaml << " null # type=NO_DATA\n";
      break;

    case DataTreeType::SCALAR:
      {
        yaml << " " << m_value.PrintStr(/*with_type=*/false);
        yaml << " # type=" << m_value.TypeName();
        yaml << "\n";
      }
      break;

    case DataTreeType::SEQUENCE:
      yaml << " # type=SEQUENCE";
      yaml << "\n";
      for (const auto& child : m_children)
      {
        yaml << child_indent << "-";
        yaml << child->toStringAsYAML(child_indent);
      }

      break;

    case DataTreeType::MAP:
      yaml << " # type=MAP\n";
      for (const auto& child : m_children)
      {
        yaml << child_indent;
        yaml << child->toStringAsYAML(indent + std::string(2, ' '));
      }
      break;
  }

  // //=================================== Branch case this is NOT a subtree
  // if (m_children.empty())
  // {
  //   //============================ Single value
  //   if (m_values.size() == 1)
  //   {
  //     yaml << " " << m_values.front().PrintStr(false) << "\n";
  //   }
  //   //============================ Array value
  //   else
  //   {
  //     yaml << "[";
  //     for (const auto& value : m_values)
  //     {
  //       yaml << value.PrintStr(/*with_type=*/false);
  //       if (value != m_values.back()) yaml << ", ";
  //     }
  //     yaml << "]\n";
  //   }
  // } // m_children.empty()
  // //=================================== Branch case: this is a subtree
  // else
  // {
  //   yaml << "\n";
  //   size_t k = 0;
  //   for (const auto& child : m_children)
  //     yaml << child->toStringAsYAML(indent + std::string(4, ' '));
  // }

  return yaml.str();
}

// ###################################################################
/**Returns a const reference to a child of only the current level tree. If the
 * name is not found std::logic_error is thrown.
 */
DataTree& DataTree::child(const std::string& child_name)
{
  if (m_children.empty())
    throw std::logic_error("Child '" + child_name + "' not found");

  for (const auto& child : m_children)
    if (child->name() == child_name) return *child;

  throw std::logic_error("Child '" + child_name + "' not found");
}

// ###################################################################
/**Returns a const reference to a child of only the current level tree. If the
 * name is not found std::logic_error is thrown.
 */
const DataTree& DataTree::child(const std::string& child_name) const
{
  if (m_children.empty())
    throw std::logic_error("Child '" + child_name + "' not found");

  for (const auto& child : m_children)
    if (child->name() == child_name) return *child;

  throw std::logic_error("Child '" + child_name + "' not found");
}

// // ###################################################################
// /**Returns a const reference to a child at any level of the tree. If
//  * the name is not found std::logic_error is thrown.*/
// const DataTree& DataTree::childByAddress(const std::string& address) const
// {
//
//   auto words = string_utils::splitString(address, "/");
//   if (words.empty()) words = {address};
//
//   // try
//   // {
//   //   child
//   // }
//   // catch (const std::exception& e)
//   // {
//   //   throw std::logic_error("Invalid address '" + address + "' in data tree
//   '" +
//   //                          m_name + "'.");
//   // }
// }

} // namespace elke