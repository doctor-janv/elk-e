#ifndef ELK_E_C_API_H
#define ELK_E_C_API_H

#include <iostream>

/**Puts a new DataTree object in the warehouse and returns a numeric handle
 * to the object.
 */
extern "C" int elke_DataTree_makeNew(int& errorCode, const char* c_str);

/**Given a handle to the data-tree, adds a subtree at the required address.*/
extern "C" void elke_DataTree_addSubTree(int& errorCode,
                                         int handle,
                                         const char* address,
                                         const char* name);

extern "C" void elke_DataTree_printYAMLString(int& errorCode, int handle);

/**Given a handle to the data-tree, adds an integer-value to the subtree at the
 * address.*/
extern "C" void elke_DataTree_setIntValue(int& errorCode,
                                          int handle,
                                          const char* address,
                                          int64_t int_value);

/**Given a handle to the data-tree, adds a real-value to the subtree at the
 * address.*/
extern "C" void elke_DataTree_setRealValue(int& errorCode,
                                           int handle,
                                           const char* address,
                                           double real_value);

/**Given a handle to the data-tree, adds a bool-value to the subtree at the
 * address.*/
extern "C" void elke_DataTree_setBoolValue(int& errorCode,
                                           int handle,
                                           const char* address,
                                           bool bool_value);

/**Given a handle to the data-tree, adds a string-value to the subtree at the
 * address.*/
extern "C" void elke_DataTree_setStringValue(int& errorCode,
                                             int handle,
                                             const char* address,
                                             const char* string_value);

extern "C" void elke_DataTree_setType(int& errorCode,
                                      const int root_handle,
                                      const char* address,
                                      const int type_id);

#endif // ELK_E_C_API_H
