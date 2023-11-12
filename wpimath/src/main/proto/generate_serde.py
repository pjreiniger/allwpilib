# import controller_pb2
import importlib
import inspect
import sys
from jinja2 import Environment, BaseLoader

# print(controller_pb2)

# clsmembers = inspect.getmembers(controller_pb2, inspect.isclass)
# print(clsmembers)

from wpimath.src.main.proto.generator.java_helpers import render_message_java
from wpimath.src.main.proto.generator.cpp_helpers import render_cpp_class

from wpimath.src.main.proto.generator.lib import MessageClass, ProtobufModule


def main():
    # proto_files = ["geometry2d", "geometry3d"]
    proto_files = [
        # ("geometry2d", "geometry"),
        # ("geometry3d", "geometry"),
          ("controller", "controller"),
          ("kinematics", "kinematics"),
        #   ("plant", "system/plant"),
         ###########33 ("spline", "spline"),
        #   ("system", "system"),
          ("trajectory", "trajectory"),
        #   ("wpimath", "."),
    ]

    modules = []

    for proto_file, output_directory in proto_files:
        # java_package = proto_file[0].upper() + proto_file[1:]
        # java_package = java_package.replace("2d", "2D")
        # java_package = java_package.replace("3d", "3D")
        module_name = proto_file + "_pb2"

        modules.append(ProtobufModule(proto_file, module_name))

    
    for module in modules:
        for message in module.messages:
            render_message_java(module, message)

if __name__ == "__main__":
    main()
