import ctypes


class ElkeDataTree:
    def __init__(self, handle: int, name: str, dll, data_dictionary: dict):
        self.name = name
        self.__handle = handle
        self.__dll = dll

        base_name = name+"/"

        def traverse_dict(input_address: str, input_dict: dict):
            for key, value in input_dict.items():
                if type(value) is list:
                    self.add_sub_tree(input_address, key)
                    for arr_value in value:
                        self.add_value(input_address+key+"/", arr_value)
                elif type(value) is not dict:
                    self.add_sub_tree(input_address, key)
                    self.add_value(input_address+key+"/", value)
                else:
                    self.add_sub_tree(input_address, key)
                    traverse_dict(input_address + key + "/", value)

        traverse_dict(base_name, data_dictionary)

        error = ctypes.c_int()
        dll.elke_DataTree_printYAMLString(ctypes.byref(error), ctypes.c_int(handle))

        if error.value:
            print(type(error.value), error.value)
            raise RuntimeError("Error")


    def add_sub_tree(self, node_address: str, name):
        handle = self.__handle
        dll = self.__dll

        error = ctypes.c_int()
        dll.elke_DataTree_addSubTree(ctypes.byref(error),
                                     ctypes.c_int(handle),
                                     node_address.encode("utf-8"),
                                     name.encode("utf-8"))

        if error.value:
            print(type(error.value), error.value)
            raise RuntimeError("Error")

    def add_value(self, node_address: str, value):
        handle = self.__handle
        dll = self.__dll

        error = ctypes.c_int()
        if type(value) is int:
            dll.elke_DataTree_addIntValue(ctypes.byref(error),
                                          ctypes.c_int(handle),
                                          node_address.encode("utf-8"),
                                          ctypes.c_int8(value))
        elif type(value) is float:
            dll.elke_DataTree_addRealValue(ctypes.byref(error),
                                           ctypes.c_int(handle),
                                           node_address.encode("utf-8"),
                                           ctypes.c_double(value))
        elif type(value) is bool:
            dll.elke_DataTree_addBoolValue(ctypes.byref(error),
                                           ctypes.c_int(handle),
                                           node_address.encode("utf-8"),
                                           ctypes.c_bool(value))

        elif type(value) is str:
            dll.elke_DataTree_addStringValue(ctypes.byref(error),
                                             ctypes.c_int(handle),
                                             node_address.encode("utf-8"),
                                             value.encode("utf-8"))
        else:
            raise RuntimeError("Unsupported type " + str(type(value)))


        if error.value:
            print(type(error.value), error.value)
            raise RuntimeError("Error")


