from google.protobuf.descriptor import FieldDescriptor
from jinja2 import Environment, FileSystemLoader
from wpimath.src.main.proto.generator.lib import MessageClass, ProtobufModule, MessageField, upper_camel_case, lower_camel_case, render_template, snake_to_camel_case
import os

PROTO_TYPE_TO_CPP_TEXT = {
    FieldDescriptor.TYPE_DOUBLE: "double",
    FieldDescriptor.TYPE_BOOL: "boolean",
    FieldDescriptor.TYPE_INT32: "int",
    FieldDescriptor.TYPE_UINT32: "int",
}


# def get_field_class_name(clazz, field):
#     obj = clazz[1]()
#     field_clazz = type(getattr(obj, field.name))
#     return field_clazz.__name__.replace("Protobuf", "")


def __get_schema(message : MessageClass):
    return ";".join(field.get_schema(PROTO_TYPE_TO_CPP_TEXT) for field in message.fields)

def __get_struct_size(message):
    primitive_size = 0
    subclass_parts = []

    output = ""

    for field in message.fields:
        if field.is_message:
            subclass_parts.append(
                f"wpi::Struct<frc::{field.message_type}>::kSize"
            )
        elif field.protobuf_type == FieldDescriptor.TYPE_DOUBLE:
            primitive_size += 8
        elif field.protobuf_type == FieldDescriptor.TYPE_INT32:
            num_doubles += 1
        elif field.protobuf_type == FieldDescriptor.TYPE_UINT32:
            num_doubles += 1
    #     elif field.type == FieldDescriptor.TYPE_MESSAGE:
    #         pass
    #     else:
    #         raise Exception(f"Unsupported type {field.type} for field {field.name}")

    if primitive_size > 0:
        output += f"{primitive_size}"

    if subclass_parts and output:
        output += "+ "
    output += " + ".join(subclass_parts)

    print("GET SIZE : " + output)

    return output

# def get_schema(clazz, fields):
#     schema_fields = []

#     for field in fields:
#         if field.type == FieldDescriptor.TYPE_MESSAGE:
#             schema_fields.append(f"{get_field_class_name(clazz, field)} {field.name}")
#         else:
#             field_type = PROTO_TYPE_TO_CPP_TEXT[field.type]
#             schema_fields.append(field_type + " " + field.name)

#     return ";".join(schema_fields)


# def strip_units(field_name):
#     field_name = field_name.replace("_meters", "")
#     field_name = field_name.replace("_radians", "")

#     return field_name

# def assert_equals(clazz, field, data_name):
#     print(field)
#     if data_name == "proto":
#         return f"EXPECT_EQ(kExpectedData.{strip_units(field.name)}.value(), {data_name}.{field.name}());"
#     return f"EXPECT_EQ(kExpectedData.{strip_units(field.name)}.value(), {data_name}.{strip_units(field.name)}.value());"
#     # if data_name == "proto" and field.type == FieldDescriptor.TYPE_MESSAGE:
#     #     return f"assertEquals(DATA.get{upper_camel_case(field.name)}(), {get_field_class_name(clazz, field)}.proto.unpack(proto.get{upper_camel_case(field.name)}()));"
#     # return f"assertEquals(DATA.get{upper_camel_case(field.name)}(), {data_name}.get{upper_camel_case(field.name)}());"

# def test_proto_setter(clazz, field):
#     return f"proto.set_{field.name}(kExpectedData.{strip_units(field.name)}.value());"
#     # if field.type == FieldDescriptor.TYPE_MESSAGE:
#     #     return f"{get_field_class_name(clazz, field)}.proto.pack(proto.getMutable{upper_camel_case(field.name)}(), DATA.get{upper_camel_case(field.name)}());"
#     # return f"proto.set{upper_camel_case(field.name)}(DATA.get{upper_camel_case(field.name)}());"


def render_message_cpp(module : ProtobufModule, message : MessageClass, force_tests: bool):
    env = Environment(
        loader=FileSystemLoader(
            "/home/pjreiniger/git/allwpilib/wpimath/src/main/proto/generator/templates"
        )
    )
    env.globals["get_size"] = __get_struct_size
    env.globals["get_schema"] = __get_schema
    # env.globals["assert_equals"] = assert_equals
    # env.globals["test_proto_setter"] = test_proto_setter

    kwargs = dict(
        module = module,
        message = message,
                # protobuf_type=protobuf_type,
                # lang_type=lang_type,
                # has_nested_types=has_nested_types,
                # clazz = clazz,
                # fields=clazz[1].DESCRIPTOR.fields,
    )
    
    lang_type = message.local_type



    wpimath_dir = "/home/pjreiniger/git/allwpilib/wpimath"


    wpimath_cpp_dir = os.path.join(wpimath_dir, "src/main/native/cpp/")
    wpimath_test_dir = os.path.join(wpimath_dir, "src/test/native/cpp/")
    # wpimath_test_dir = os.path.join(wpimath_dir, "src/test/java/edu/wpi/first/math")


    wpimath_incl_dir = os.path.join(wpimath_dir, "src/main/native/include/frc")
    wpimath_incl_serde_dir = os.path.join(wpimath_incl_dir, module.subfolder, "serde")
    proto_src = os.path.join(wpimath_incl_serde_dir, f"{lang_type}Serde.inc")
    render_template(env, "cpp_serde.h.jinja2", proto_src, **kwargs)
    # with open(proto_src, "w") as f:
    #     f.write(env.get_template("cpp_proto_serde.h.jinja2").render(**kwargs))

    # proto_src = os.path.join(wpimath_cpp_dir, f"{output_directory}/{lang_type}ProtoSerde.cpp")
    # with open(proto_src, "w") as f:
    #     f.write(env.get_template("cpp_proto_serde.cpp.jinja2").render(**kwargs))



    # proto_src = os.path.join(wpimath_incl_dir, f"{output_directory}/{lang_type}StructSerde.h")
    # with open(proto_src, "w") as f:
    #     f.write(env.get_template("cpp_struct_serde.h.jinja2").render(**kwargs))

    # proto_src = os.path.join(wpimath_cpp_dir, f"{output_directory}/{lang_type}StructSerde.cpp")
    # with open(proto_src, "w") as f:
    #     f.write(env.get_template("cpp_struct_serde.cpp.jinja2").render(**kwargs))


    # test_src = os.path.join(wpimath_test_dir, f"{output_directory}/serde/{lang_type}SerdeTest.cpp")
    # with open(test_src, "w") as f:
    #     f.write(env.get_template("cpp_serde_test.jinja2").render(**kwargs))
