
import importlib
import inspect
import sys

from google.protobuf.descriptor import FieldDescriptor

class ProtobufModule:
    def __init__(self, subfolder, module_name):
        importlib.import_module(module_name, package=None)
        clsmembers = inspect.getmembers(sys.modules[module_name], inspect.isclass)

        self.subfolder = subfolder
        self.java_proto_subpath = subfolder[0].upper() + subfolder[1:]
        self.messages = []

        for clazz in clsmembers:
            self.messages.append(MessageClass(module_name, clazz[0], clazz[1]))




class MessageClass:
    def __init__(self, module_name, protobuf_type, protobuf_class):
        # print(protobuf_type, protobuf_type)
        self.protobuf_short_type = protobuf_type
        self.has_nested_types = False
        self.local_type = protobuf_type[len("Protobuf") :]

        self.fields : MessageField = []
        
        for field in protobuf_class.DESCRIPTOR.fields:
            if field.type == FieldDescriptor.TYPE_MESSAGE:
                self.has_nested_types = True
            
            # print("  ", field)
            self.fields.append(MessageField(protobuf_class, field))

        


class MessageField:
    def __init__(self, protobuf_class, protobuf_field : FieldDescriptor):
        # print("  ", protobuf_field)
        self.name = protobuf_field.name
        self.protobuf_type = protobuf_field.type
        self.is_message = self.protobuf_type == FieldDescriptor.TYPE_MESSAGE
        if self.is_message:
            field_clazz = type(getattr(protobuf_class(), self.name))
            # print(field_clazz)
            self.message_type = field_clazz.__name__.replace("Protobuf", "")

        self.name_without_units = self.name
        self.name_without_units = self.name_without_units.replace("_volts", "")
        self.name_without_units = self.name_without_units.replace("_meters", "")
        self.name_without_units = self.name_without_units.replace("_mps", "")
        self.name_without_units = self.name_without_units.replace("_rps", "")

    def get_schema(self, primitive_lookup):
        if self.is_message:
            return self.message_type + " " + self.name_without_units
        
        return primitive_lookup[self.protobuf_type] + " " + self.name_without_units