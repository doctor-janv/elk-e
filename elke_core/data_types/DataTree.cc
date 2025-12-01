#include "DataTree.h"

#include <utility>
#include <sstream>

namespace elke
{
/**Constructor requiring the name.*/
DataTree::DataTree(std::string name) : m_name(std::move(name))
{
  m_tags["address"] = m_name;
}

/**Returns the general type of the data-tree.*/
DataTreeType DataTree::type() const
{
  return m_type;
}

/**Returns a string representation of the type.*/
std::string DataTreeTypeName(const DataTreeType type)
{
  switch (type)
  {
    case DataTreeType::NO_DATA:
      return "no_data";
    case DataTreeType::SCALAR:
      return "scalar";
    case DataTreeType::SEQUENCE:
      return "sequence";
    case DataTreeType::MAP:
      return "map";
    default:
      return "no_data";
  }
}


/**Sets the data-tree type.*/
void DataTree::setType(const DataTreeType type)
{
  m_tags["type"] = DataTreeTypeName(type);
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

  m_tags["type"] = value.TypeName();
}

// ###################################################################
/**Adds a child tree.*/
void DataTree::addChild(const DataTreePtr& child)
{
  if (not (m_type == DataTreeType::SEQUENCE or m_type == DataTreeType::MAP))
    throw std::runtime_error(
      "Attempting to add child to DataTree " + m_name +
      " which is not designated as either a SEQUENCE or a MAP.");
  const size_t id = m_children.size();
  m_children.push_back(child);

  const auto address_tag = m_tags.find("address");
  if (address_tag == m_tags.end())
    throw std::runtime_error("Address corruption.");

  if (m_type == DataTreeType::SEQUENCE)
  {
    const std::string name = std::to_string(id);
    child->setAddressTag(address_tag->second + "/" + name);
  }
  else
    child->setAddressTag(address_tag->second + "/" + child->name());
}

// ###################################################################
void DataTree::setAddressTag(const std::string& address)
{
  m_tags["address"] = address;
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

  auto appendTags = [&]
  {
    yaml << " # ";
    for (const auto& [tag_key, value] : m_tags)
    {
      bool print_tag = false;

      if (tag_key == "type") print_tag = true;

      if (print_tag) yaml << tag_key << "=" << value << " ";
    }
  };

  switch (this->type())
  {
    case DataTreeType::NO_DATA:
      yaml << " null";
      appendTags();
      yaml << "\n";
      break;

    case DataTreeType::SCALAR:
      {
        yaml << " " << m_value.PrintStr(/*with_type=*/false);
        appendTags();
        yaml << "\n";
      }
      break;

    case DataTreeType::SEQUENCE:
      appendTags();
      yaml << "\n";
      for (const auto& child : m_children)
      {
        yaml << child_indent << "-";
        yaml << child->toStringAsYAML(child_indent);
      }

      break;

    case DataTreeType::MAP:
      appendTags();
      yaml << "\n";
      for (const auto& child : m_children)
      {
        yaml << child_indent;
        yaml << child->toStringAsYAML(indent + std::string(2, ' '));
      }
      break;
  }

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


} // namespace elke