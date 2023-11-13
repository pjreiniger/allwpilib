from google.protobuf.descriptor import FieldDescriptor
from jinja2 import Environment, FileSystemLoader
import os

PROTO_TYPE_TO_CPP_TEXT = {
    FieldDescriptor.TYPE_DOUBLE: "double",
    FieldDescriptor.TYPE_BOOL: "boolean",
    FieldDescriptor.TYPE_INT32: "int",
    FieldDescriptor.TYPE_UINT32: "int",
}


def get_field_class_name(clazz, field):
    obj = clazz[1]()
    field_clazz = type(getattr(obj, field.name))
    return field_clazz.__name__.replace("Protobuf", "")

def get_size(clazz, fields):
    primitive_size = 0
    subclass_parts = []

    output = ""

    for field in fields:
        print(field)
        if field.type == FieldDescriptor.TYPE_DOUBLE:
            primitive_size += 8
        # elif field.type == FieldDescriptor.TYPE_INT32:
        #     num_doubles += 1
        # elif field.type == FieldDescriptor.TYPE_UINT32:
        #     num_doubles += 1
        elif field.type == FieldDescriptor.TYPE_MESSAGE:
            subclass_parts.append(
                f"wpi::Struct<frc::{get_field_class_name(clazz, field)}>::kSize"
            )
        else:
            raise Exception(f"Unsupported type {field.type} for field {field.name}")

    if primitive_size > 0:
        output += f"{primitive_size}"

    if subclass_parts and output:
        output += "+ "
    output += " + ".join(subclass_parts)

    print("GET SIZE : " + output)

    return output

def get_schema(clazz, fields):
    schema_fields = []

    for field in fields:
        if field.type == FieldDescriptor.TYPE_MESSAGE:
            schema_fields.append(f"{get_field_class_name(clazz, field)} {field.name}")
        else:
            field_type = PROTO_TYPE_TO_CPP_TEXT[field.type]
            schema_fields.append(field_type + " " + field.name)

    return ";".join(schema_fields)


def strip_units(field_name):
    field_name = field_name.replace("_meters", "")
    field_name = field_name.replace("_radians", "")

    return field_name

def assert_equals(clazz, field, data_name):
    print(field)
    if data_name == "proto":
        return f"EXPECT_EQ(kExpectedData.{strip_units(field.name)}.value(), {data_name}.{field.name}());"
    return f"EXPECT_EQ(kExpectedData.{strip_units(field.name)}.value(), {data_name}.{strip_units(field.name)}.value());"
    # if data_name == "proto" and field.type == FieldDescriptor.TYPE_MESSAGE:
    #     return f"assertEquals(DATA.get{upper_camel_case(field.name)}(), {get_field_class_name(clazz, field)}.proto.unpack(proto.get{upper_camel_case(field.name)}()));"
    # return f"assertEquals(DATA.get{upper_camel_case(field.name)}(), {data_name}.get{upper_camel_case(field.name)}());"

def test_proto_setter(clazz, field):
    return f"proto.set_{field.name}(kExpectedData.{strip_units(field.name)}.value());"
    # if field.type == FieldDescriptor.TYPE_MESSAGE:
    #     return f"{get_field_class_name(clazz, field)}.proto.pack(proto.getMutable{upper_camel_case(field.name)}(), DATA.get{upper_camel_case(field.name)}());"
    # return f"proto.set{upper_camel_case(field.name)}(DATA.get{upper_camel_case(field.name)}());"


def render_cpp_class(output_directory, clazz, force_tests: bool):
    has_nested_types = False
    for field in clazz[1].DESCRIPTOR.fields:
        if field.type == FieldDescriptor.TYPE_MESSAGE:
            has_nested_types = True

    protobuf_type = clazz[0]
    lang_type = protobuf_type[len("Protobuf") :]

    env = Environment(
        loader=FileSystemLoader(
            "/home/pjreiniger/git/allwpilib/wpimath/src/main/proto/generator/templates"
        )
    )
    env.globals["get_size"] = get_size
    env.globals["get_schema"] = get_schema
    env.globals["assert_equals"] = assert_equals
    env.globals["test_proto_setter"] = test_proto_setter

    kwargs = dict(
                protobuf_type=protobuf_type,
                lang_type=lang_type,
                has_nested_types=has_nested_types,
                clazz = clazz,
                fields=clazz[1].DESCRIPTOR.fields,
    )



    wpimath_dir = "/home/pjreiniger/git/allwpilib/wpimath"
    wpimath_incl_dir = os.path.join(wpimath_dir, "src/main/native/include/frc")
    wpimath_cpp_dir = os.path.join(wpimath_dir, "src/main/native/cpp/")
    wpimath_test_dir = os.path.join(wpimath_dir, "src/test/native/cpp/")
    # wpimath_test_dir = os.path.join(wpimath_dir, "src/test/java/edu/wpi/first/math")


    # proto_src = os.path.join(wpimath_incl_dir, f"{output_directory}/{lang_type}ProtoSerde.h")
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


    test_src = os.path.join(wpimath_test_dir, f"{output_directory}/serde/{lang_type}SerdeTest.cpp")
    with open(test_src, "w") as f:
        f.write(env.get_template("cpp_serde_test.jinja2").render(**kwargs))
