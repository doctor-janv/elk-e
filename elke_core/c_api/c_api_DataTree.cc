#include "c_api_DataTree.h"

#include "elke_core/FrameworkCore.h"
#include "elke_core/data_types/DataTree.h"
#include "elke_core/base/Warehouse.h"

#include <string>
#include <memory>

//===================================================================
int elke_DataTree_makeNew(int& errorCode, const char* c_str)
{
  errorCode = 0;
  try
  {
    std::cout << "Making new data tree " << c_str << std::endl;
    const auto new_data_tree = std::make_shared<elke::DataTree>(c_str);

    new_data_tree->setGrossType(elke::DataGrossType::MAP);

    auto& warehouse = elke::FrameworkCore::getInstance().warehouse();

    const size_t handle =
      warehouse.DataTreeStorage().depositItem(new_data_tree);

    return static_cast<int>(handle);
  }
  catch (std::exception& e)
  {
    errorCode = 1;
    std::cout << e.what();
    return 0;
  }
}

void elke_DataTree_setType(int& errorCode,
                           const int root_handle,
                           const char* address,
                           const int type_id)
{
  errorCode = 0;
  try
  {
    const auto str_address = std::string(address);

    auto& warehouse = elke::FrameworkCore::getInstance().warehouse();
    auto& stack = warehouse.DataTreeStorage();

    elke::DataTree& root_tree =
      stack.getItemReference(static_cast<size_t>(root_handle));

    //=========================================== Call back function
    auto callback =
      [&](const std::string& current_address, elke::DataTree& current_tree)
    {
      if (current_address == str_address)
      {
        switch (type_id)
        {
          case 1:
            current_tree.setGrossType(elke::DataGrossType::SCALAR);
            break;
          case 2:
            current_tree.setGrossType(elke::DataGrossType::SEQUENCE);
            break;
          case 3:
            current_tree.setGrossType(elke::DataGrossType::MAP);
            break;
          default:
            current_tree.setGrossType(elke::DataGrossType::NO_DATA);
        }
      } // if
    }; // End of callback

    root_tree.traverseWithCallback("", callback);
  }
  catch (std::exception& e)
  {
    errorCode = 1;
    std::cout << e.what();
  }
}

void elke_DataTree_printYAMLString(int& errorCode, const int handle)
{
  errorCode = 0;
  try
  {
    auto& warehouse = elke::FrameworkCore::getInstance().warehouse();
    auto& stack = warehouse.DataTreeStorage();

    const elke::DataTree& tree =
      stack.getItemReference(static_cast<size_t>(handle));
    std::cout << tree.toStringAsYAML("");
  }
  catch (std::exception& e)
  {
    errorCode = 1;
    std::cout << e.what();
  }
}

//===================================================================
void elke_DataTree_addSubTree(int& errorCode,
                              const int handle,
                              const char* address,
                              const char* name)
{
  errorCode = 0;
  try
  {
    const auto str_address = std::string(address);
    const auto str_name = std::string(name);

    auto& warehouse = elke::FrameworkCore::getInstance().warehouse();
    auto& stack = warehouse.DataTreeStorage();

    elke::DataTree& tree = stack.getItemReference(static_cast<size_t>(handle));

    // Traverse the tree until the address is found
    bool address_found = false;

    //=========================================== Call back function
    auto callback =
      [&](const std::string& current_address, elke::DataTree& current_tree)
    {
      if (current_address == str_address)
      {
        const std::string allowed_name =
          current_tree.grossType() == elke::DataGrossType::MAP ? str_name : "";
        current_tree.addChild(std::make_shared<elke::DataTree>(allowed_name));
        address_found = true;
      }
    }; // End of callback

    tree.traverseWithCallback("", callback);

    if (not address_found)
      throw std::runtime_error("Could not find address " + str_address);
  }
  catch (std::exception& e)
  {
    errorCode = 1;
    std::cout << e.what() << std::endl;
  }
}

namespace elke
{
/**This is a utility function only used here.
 *Given a handle to the data-tree, adds a template-value to the subtree at the
 * address.*/
template <typename T>
void DataTree_setArbitraryValue(int& errorCode,
                                const int handle,
                                const char* address,
                                const T value)
{
  errorCode = 0;
  try
  {
    const auto str_address = std::string(address);

    auto& warehouse = elke::FrameworkCore::getInstance().warehouse();
    auto& stack = warehouse.DataTreeStorage();

    DataTree& tree = stack.getItemReference(static_cast<size_t>(handle));

    // Traverse the tree until the address is found
    bool address_found = false;

    auto callback =
      [&](const std::string& current_address, DataTree& current_tree)
    {
      if (current_address == str_address)
      {
        current_tree.setValue(Varying(value));
        address_found = true;
      }
    };

    tree.traverseWithCallback("", callback);

    if (not address_found)
      throw std::runtime_error("Could not find address " + str_address);
  }
  catch (std::exception& e)
  {
    errorCode = 1;
    std::cout << e.what() << std::endl;
  }
}

} // namespace elke

//===================================================================
void elke_DataTree_setIntValue(int& errorCode,
                               const int handle,
                               const char* address,
                               const int64_t int_value)
{
  elke::DataTree_setArbitraryValue(errorCode, handle, address, int_value);
}

//===================================================================
void elke_DataTree_setRealValue(int& errorCode,
                                const int handle,
                                const char* address,
                                const double real_value)
{
  elke::DataTree_setArbitraryValue(errorCode, handle, address, real_value);
}

//===================================================================
void elke_DataTree_setBoolValue(int& errorCode,
                                const int handle,
                                const char* address,
                                const bool bool_value)
{
  elke::DataTree_setArbitraryValue(errorCode, handle, address, bool_value);
}

//===================================================================
void elke_DataTree_setStringValue(int& errorCode,
                                  const int handle,
                                  const char* address,
                                  const char* string_value)
{
  elke::DataTree_setArbitraryValue(errorCode, handle, address, string_value);
}