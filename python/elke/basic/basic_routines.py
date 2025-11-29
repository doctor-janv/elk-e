import ctypes
from .ElkeDataTree import *

class ElkePythonInterface:
    def __init__(self, library_path):
        print("Initializing elke")

        # ctypes.cdll.LoadLibrary(library_path)
        dll = ctypes.CDLL(library_path)

        dll.elke_FrameworkCore_initialize()

        self.__dll = dll


    def make_data_tree(self, name: str, data_dictionary: dict) -> ElkeDataTree:
        dll = self.__dll

        error = ctypes.c_int()
        handle = dll.elke_DataTree_makeNew(ctypes.byref(error), name.encode("utf-8"))

        if error.value:
            print(type(error.value), error.value)
            raise RuntimeError("Error")

        return ElkeDataTree(handle, name, dll, data_dictionary)

