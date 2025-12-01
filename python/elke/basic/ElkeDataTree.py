import ctypes

from enum import IntEnum

class ElkeDataTreeType(IntEnum):
    SCALAR = 1
    SEQUENCE = 2
    MAP = 3

class ElkeDataTree:
    def __init__(self, handle: int, name: str, dll, data_dictionary: dict):
        self.name = name
        self.__handle = handle
        self.__dll = dll

        base_address = name+"/"

        def traverse_dict(input_address: str, node_name, node):
            if type(node) in [str, int, float, bool]:
                self.add_sub_tree(input_address, node_name, ElkeDataTreeType.SCALAR)
                self.set_value(input_address+node_name+"/", node)
            elif type(node) is list:
                self.add_sub_tree(input_address, node_name, ElkeDataTreeType.SEQUENCE)
                k = 0
                for item in node:
                    key = str(k)
                    traverse_dict(input_address + node_name + "/", key, item)
                    k += 1
            elif type(node) is dict:
                self.add_sub_tree(input_address, node_name, ElkeDataTreeType.MAP)
                for key, value in node.items():
                    traverse_dict(input_address + node_name + "/", key, value)
            else:
                raise RuntimeError("Unsupported type.")

        for main_key, main_value in data_dictionary.items():
            traverse_dict(base_address, main_key, main_value)

        error = ctypes.c_int()
        dll.elke_DataTree_printYAMLString(ctypes.byref(error), ctypes.c_int(handle))

        if error.value:
            print(type(error.value), error.value)
            raise RuntimeError("Error")


    def add_sub_tree(self, node_address: str, name, tree_type: int):
        handle = self.__handle
        dll = self.__dll

        error = ctypes.c_int()
        dll.elke_DataTree_addSubTree(ctypes.byref(error),
                                     ctypes.c_int(handle),
                                     node_address.encode("utf-8"),
                                     name.encode("utf-8"))

        if error.value:
            raise RuntimeError("Error"+str(error.value))
        # print("***Added " + name + " type=" + str(tree_type) + f" at address \"{node_address}\"", flush=True)

        error = ctypes.c_int()

        # print(f"***Setting type at address {node_address+name} to {tree_type}", flush=True)
        dll.elke_DataTree_setType(ctypes.byref(error),
                                  ctypes.c_int(handle),
                                  (node_address+name+"/").encode("utf-8"),
                                  ctypes.c_int(tree_type))

        if error.value:
            raise RuntimeError("Error"+str(error.value))

    def set_value(self, node_address: str, value):
        handle = self.__handle
        dll = self.__dll

        error = ctypes.c_int()
        if type(value) is int:
            dll.elke_DataTree_setIntValue(ctypes.byref(error),
                                          ctypes.c_int(handle),
                                          node_address.encode("utf-8"),
                                          ctypes.c_int8(value))
        elif type(value) is float:
            dll.elke_DataTree_setRealValue(ctypes.byref(error),
                                           ctypes.c_int(handle),
                                           node_address.encode("utf-8"),
                                           ctypes.c_double(value))
        elif type(value) is bool:
            dll.elke_DataTree_setBoolValue(ctypes.byref(error),
                                           ctypes.c_int(handle),
                                           node_address.encode("utf-8"),
                                           ctypes.c_bool(value))

        elif type(value) is str:
            dll.elke_DataTree_setStringValue(ctypes.byref(error),
                                             ctypes.c_int(handle),
                                             node_address.encode("utf-8"),
                                             value.encode("utf-8"))
        else:
            raise RuntimeError("Unsupported type " + str(type(value)))


        if error.value:
            print(type(error.value), error.value)
            raise RuntimeError("Error")


