import sys

sys.path.append("python/elke")

from elke import *

elke_lib = ElkePythonInterface(library_path="lib/libelke.dylib")

input_data = dict(
    component_100 = dict(
        type = "snglvol",
        xgeometry = dict(area = 0.25, length = 1.0),
        initial_conditions = dict(pressure = 1.0e6, temperature = 300.0),
        param1 = [2,3,4],
        param2 = ["abc", "def"],
        param3 = ["mixed", 123, 45.6]
    )
)

tree = elke_lib.make_data_tree("MainInput", input_data)
