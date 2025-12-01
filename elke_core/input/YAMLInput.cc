#include "YAMLInput.h"

#include "elke_core/output/Logger.h"

#ifdef YAML_CPP_EXISTS
#include "yaml-cpp/yaml.h"
#endif

namespace elke
{


#ifdef YAML_CPP_EXISTS
namespace YAMLInputHelpers
{
// ###################################################################
/**Called when YAML.type() == Scalar.*/
void populateValue(elke::DataTree& parent_tree,
                   const YAML::Node& node,
                   Logger& logger,
                   const int level,
                   const bool test_mode)
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
    try { [[maybe_unused]] const auto _ = node.as<int64_t>(); }
    catch (const YAML::Exception&) { is_integer = false; }

    if (is_integer)
    {
      type = "[integer]";
      parent_tree.setValue(Varying(node.as<int64_t>()));
    }
    else
    {
      type = "[real]";
      parent_tree.setValue(Varying(node.as<double>()));
    }
  }

  //=================================== Test for boolean
  bool is_boolean = true;
  try { [[maybe_unused]] const auto _ = node.as<bool>(); }
  catch (const YAML::Exception&) { is_boolean = false; }

  if (is_boolean)
  {
    type = "[boolean]";
    parent_tree.setValue(Varying(node.as<bool>()));
  }

  //=================================== Default to string
  if (not is_number and not is_boolean)
  {
    type = "[string]";
    parent_tree.setValue(Varying(node.as<std::string>()));
  }

  if (test_mode) logger.log() << offset << "Scalar node " << node.Tag()
               << " " + parent_tree.name() + ":" << " "
               << node.as<std::string>() << " " + type;

  // clang-format on
}
} // namespace YAMLInputHelpers

// ###################################################################
/**Recursive function to run through a YAML-tree.*/
void YAMLInput::populateTree(elke::DataTree& tree,
                             const YAML::Node& node,
                             Logger& logger,
                             const int level,
                             const bool test_mode)
{
  const auto offset = std::string(level + 2, ' ');
  const auto mark = node.Mark();

  tree.setTag("mark",
              m_current_file_name + " line " + std::to_string(mark.line + 1) + ":" +
                std::to_string(mark.column + 1));

  switch (node.Type())
  {
    case YAML::NodeType::Null:
      if (test_mode) logger.log() << offset << "Null node\n";
      tree.setType(DataTreeType::NO_DATA);
      break;
    case YAML::NodeType::Scalar:
      tree.setType(DataTreeType::SCALAR);
      YAMLInputHelpers::populateValue(tree, node, logger, level, test_mode);
      break;
    case YAML::NodeType::Sequence:
      if (test_mode) logger.log() << offset << "Sequence node\n";
      tree.setType(DataTreeType::SEQUENCE);
      for (size_t i = 0; i < node.size(); i++)
      {
        auto sub_node_ptr = std::make_shared<DataTree>("");
        auto& sub_node = *sub_node_ptr;
        tree.addChild(sub_node_ptr);
        populateTree(sub_node, node[i], logger, level + 2, test_mode);
      }
      break;
    case YAML::NodeType::Map:
      if (test_mode) logger.log() << offset << "Map node\n";
      tree.setType(DataTreeType::MAP);
      for (auto it = node.begin(); it != node.end(); ++it)
      {
        auto sub_node_ptr =
          std::make_shared<DataTree>(it->first.as<std::string>());
        auto& sub_node = *sub_node_ptr;
        tree.addChild(sub_node_ptr);
        populateTree(sub_node, it->second, logger, level + 2, test_mode);
      }
      break;
    case YAML::NodeType::Undefined:
      throw std::logic_error("YAML node is not defined");
  }
}
#endif


// ###################################################################
YAMLInput::YAMLInput(Logger& logger, const bool test_mode /*=false*/)
  : InputParser(logger), m_test_mode(test_mode)
{
}

// ###################################################################
/**Parses input files into data trees using a file-appropriate parser.*/
elke::DataTree YAMLInput::parseInputFile(const std::string file_name)
{
  elke::DataTree data_tree("");

  m_current_file_name = file_name;

#ifdef YAML_CPP_EXISTS
  m_logger.log() << "Reading YAML-file \"" << file_name << "\"\n";
  const YAML::Node root = YAML::LoadFile(file_name);

  DataTree root_tree(file_name);
  try
  {
    this->populateTree(root_tree, root, m_logger, 0, m_test_mode);
  }
  catch (const std::exception& e)
  {
    m_errors.emplace_back(e.what());
  }
  data_tree = root_tree;
#endif

  return data_tree;
}

} // namespace elke
