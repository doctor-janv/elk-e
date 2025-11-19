#include "YAMLInput.h"

#ifdef YAML_CPP_EXISTS
#include "yaml-cpp/yaml.h"
#endif

namespace elke
{

YAMLInput::YAMLInput(Logger& logger) : m_logger(logger) {}

//###################################################################
/**Called when YAML.Type() == Scalar.*/
void populateValue(elke::DataTree& parent_tree,
                   const YAML::Node& node,
                   Logger& logger,
                   int level = 0)
{
  // clang-format off

  const auto offset = std::string(level + 2, ' ');
  std::string type;
  //=================================== Test for being a number
  // The YAML parser will be able to convert both an integer
  // and a real to a real. This will indicate a number is present
  bool is_number = true;

  try { [[maybe_unused]] const auto _ = node.as<double>(); }
  catch (const YAML::Exception&) { is_number = false;}

  //=================================== Override using the tag
  // Sometimes a yaml node might be "3" instead of just 3,
  // when this is the case the above conversion would still result in
  // a number but the tag will be set to "!"
  if (node.Tag() == "!") is_number = false;

  // The YAML parser will not convert a real to an integer, we
  // can use that fact to discern between an integer and a real
  if (is_number)
  {
    bool is_integer = true;
    try { [[maybe_unused]] const auto _ = node.as<int>(); }
    catch (const YAML::Exception&) { is_integer = false; }

    type = is_integer ? "[integer]" : "[real]";
  }

  //=================================== Test for boolean
  bool is_boolean = true;
  try { [[maybe_unused]] const auto _ = node.as<bool>(); }
  catch (const YAML::Exception&) { is_boolean = false; }

  if (is_boolean) type = "[boolean]";

  //=================================== Default to string
  if (not is_number and not is_boolean) type = "[string]";

  logger.log() << offset << "Scalar node " << node.Tag()
               << " " + parent_tree.name() + ":" << " "
               << node.as<std::string>() << " " + type;

  // clang-format on
}

//###################################################################
/**Recursive function to run through a YAML-tree.*/
void populateTree(elke::DataTree& tree,
                  const YAML::Node& node,
                  Logger& logger,
                  const int level = 0)
{
  const auto offset = std::string(level + 2, ' ');
  switch (node.Type())
  {
    case YAML::NodeType::Null:
      logger.log() << offset << "Null node\n";
      break;
    case YAML::NodeType::Scalar:
      populateValue(tree, node, logger, level);

      break;
    case YAML::NodeType::Sequence:
      logger.log() << offset << "Sequence node\n";
      for (size_t i = 0; i < node.size(); i++)
      {
        DataTree sub_node(std::to_string(i));
        populateTree(sub_node, node[i], logger, level + 2);
      }
      break;
    case YAML::NodeType::Map:
      logger.log() << offset << "Map node\n";
      for (YAML::const_iterator it = node.begin(); it != node.end(); ++it)
      {
        DataTree sub_node(it->first.as<std::string>());
        populateTree(sub_node, it->second, logger, level + 2);
      }
      break;
    case YAML::NodeType::Undefined:
      throw std::logic_error("YAML node is not defined");
  }
}

elke::DataTree YAMLInput::readInputFile(std::string file_name)
{
  elke::DataTree data_tree("");

#ifdef YAML_CPP_EXISTS
  // std::stringstream outstr;
  m_logger.log() << "Reading YAML-file \"" << file_name << "\"\n";
  const YAML::Node root = YAML::LoadFile(file_name);

  DataTree root_tree(file_name);
  populateTree(root_tree, root, m_logger);

#endif

  return data_tree;
}

} // namespace elke
