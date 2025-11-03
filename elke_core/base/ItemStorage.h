#ifndef ELK_E_ITEMSTACK_H
#define ELK_E_ITEMSTACK_H

#include <map>
#include <memory>

namespace elke
{

/**An ItemStorage is a templated class that handles the storage of items.*/
template <class T>
class ItemStorage
{
  size_t m_next_item_id = 1;
  std::map<size_t, std::shared_ptr<T>> m_items;

public:
  /**Deposits an item into the stack.*/
  size_t depositItem(std::shared_ptr<T> item)
  {
    m_items.insert(std::make_pair(m_next_item_id, item));
    return m_next_item_id++;
  }

  /**Obtains the smart pointer for an item from the stack.*/
  std::shared_ptr<T> getItemSharedPtr(size_t item_id)
  {
    auto find_result = m_items.find(item_id);
    if (find_result == m_items.end())
      throw std::logic_error("Item-id " + std::to_string(item_id) +
                             " not in stack.");

    return find_result->second;
  }
  /**Obtains a reference to an item from the stack.*/
  T& getItemReference(size_t item_id)
  {
    auto find_result = m_items.find(item_id);
    if (find_result == m_items.end())
      throw std::logic_error("Item-id " + std::to_string(item_id) +
                             " not in stack.");

    return *find_result->second;
  }
};

} // namespace elke

#endif // ELK_E_ITEMSTACK_H
