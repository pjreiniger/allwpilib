from google.protobuf.descriptor import FieldDescriptor
from jinja2 import Environment, FileSystemLoader
import os


PROTO_TYPE_TO_JAVA_TEXT = {
    FieldDescriptor.TYPE_DOUBLE: "double",
    FieldDescriptor.TYPE_BOOL: "boolean",
    FieldDescriptor.TYPE_INT32: "int",
    FieldDescriptor.TYPE_UINT32: "int",
}

print("CONVERSION LOOKUP", PROTO_TYPE_TO_JAVA_TEXT)


def upper_camel_case(name):
    name = to_camel_case(name)
    return name[0].upper() + name[1:]


def get_field_class_name(clazz, field):
    obj = clazz[1]()
    field_clazz = type(getattr(obj, field.name))
    return field_clazz.__name__.replace("Protobuf", "")

def strip_units(field_name):
    field_name = field_name.replace("_meters", "")
    field_name = field_name.replace("_radians", "")

    return field_name

def java_struct_unpack(clazz, field):
    if field.type == FieldDescriptor.TYPE_MESSAGE:
        return f"{get_field_class_name(clazz, field)} {field.name} = {get_field_class_name(clazz, field)}.struct.unpack(bb);"
    else:
        field_type = PROTO_TYPE_TO_JAVA_TEXT[field.type]
        ucc_name = upper_camel_case(field_type)
        return field_type + " " + strip_units(field.name) + " = bb.get" + ucc_name + "();"


def java_struct_pack(clazz, field):
    if field.type == FieldDescriptor.TYPE_MESSAGE:
        return f"{get_field_class_name(clazz, field)}.struct.pack(bb, value.get{upper_camel_case(field.name)}());"
    else:
        field_type = PROTO_TYPE_TO_JAVA_TEXT[field.type]
        return "bb.put" + upper_camel_case(field_type) + f"(value.get{upper_camel_case(strip_units(field.name))}());"
        # return "bb.put" + upper_camel_case(field_type) + f"(value.{strip_units(field.name)});"
        

def java_get_schema(clazz, fields):
    schema_fields = []

    for field in fields:
        if field.type == FieldDescriptor.TYPE_MESSAGE:
            schema_fields.append(f"{get_field_class_name(clazz, field)} {field.name}")
        else:
            field_type = PROTO_TYPE_TO_JAVA_TEXT[field.type]
            schema_fields.append(field_type + " " + strip_units(field.name))

    return ";".join(schema_fields)


def java_get_size(clazz, fields):
    num_doubles = 0
    subclass_parts = []

    output = ""

    for field in fields:
        if field.type == FieldDescriptor.TYPE_DOUBLE:
            num_doubles += 1
        elif field.type == FieldDescriptor.TYPE_INT32:
            num_doubles += 1
        elif field.type == FieldDescriptor.TYPE_UINT32:
            num_doubles += 1
        elif field.type == FieldDescriptor.TYPE_MESSAGE:
            subclass_parts.append(
                f"{get_field_class_name(clazz, field)}.struct.getSize()"
            )
        else:
            raise Exception(f"Unsupported type {field.type} for field {field.name}")

    if num_doubles > 0:
        output += f"kSizeDouble * {num_doubles}"

    if subclass_parts and output:
        output += "+ "
    output += " + ".join(subclass_parts)

    return output


def java_protobuf_unpack(clazz, fields):
    output_fields = []

    for field in fields:
        if field.type == FieldDescriptor.TYPE_MESSAGE:
            output_fields.append(
                f"\n        {get_field_class_name(clazz, field)}.proto.unpack(msg.get{upper_camel_case(field.name)}())"
            )
        else:
            output_fields.append("msg.get" + upper_camel_case(field.name) + "()")

    return ", ".join(output_fields)

def to_camel_case(snake_str):
    return "".join(x.capitalize() for x in snake_str.lower().split("_"))

def java_protobuf_pack(clazz, fields):
    output = ""

    for field in fields:
        if field.type == FieldDescriptor.TYPE_MESSAGE:
            obj = clazz[1]()
            field_clazz = type(getattr(obj, field.name))
            lang_class_name = field_clazz.__name__.replace("Protobuf", "")

            output += f"    {lang_class_name}.proto.pack(msg.getMutable{upper_camel_case(field.name)}(), value.get{upper_camel_case(field.name)}());\n"
        else:
            output += (
                "    msg.set"
                + upper_camel_case(field.name)
                + f"(value.get{upper_camel_case(strip_units(field.name))}());\n"
            )
            # output += "    msg.set" + upper_camel_case(field.name) + f"(value.{field.name});\n"

    return output


def java_get_nested(clazz, field, nested_type):
    output_parts = []

    for field in clazz[1].DESCRIPTOR.fields:
        if field.type == FieldDescriptor.TYPE_MESSAGE:
            output_parts.append(f"{get_field_class_name(clazz, field)}.{nested_type}")

    return "{" + ", ".join(output_parts) + "}"


def java_assert_equals(clazz, field, data_name):
    if data_name == "proto" and field.type == FieldDescriptor.TYPE_MESSAGE:
        return f"assertEquals(DATA.get{upper_camel_case(field.name)}(), {get_field_class_name(clazz, field)}.proto.unpack(proto.get{upper_camel_case(field.name)}()));"
    return f"assertEquals(DATA.get{upper_camel_case(field.name)}(), {data_name}.get{upper_camel_case(field.name)}());"

def test_proto_setter(clazz, field):
    if field.type == FieldDescriptor.TYPE_MESSAGE:
        return f"{get_field_class_name(clazz, field)}.proto.pack(proto.getMutable{upper_camel_case(field.name)}(), DATA.get{upper_camel_case(field.name)}());"
    return f"proto.set{upper_camel_case(field.name)}(DATA.get{upper_camel_case(field.name)}());"


def render_java_class(output_directory, java_package, clazz):
    has_nested_types = False
    for field in clazz[1].DESCRIPTOR.fields:
        if field.type == FieldDescriptor.TYPE_MESSAGE:
            has_nested_types = True

    # print(clazz)
    # print("  nested? ", has_nested_types)

    protobuf_type = clazz[0]
    lang_type = protobuf_type[len("Protobuf") :]
    print(clazz[0], lang_type)

    env = Environment(
        loader=FileSystemLoader(
            "/home/pjreiniger/git/allwpilib/wpimath/src/main/proto/generator/templates"
        )
    )
    env.globals["struct_unpack"] = java_struct_unpack
    env.globals["struct_pack"] = java_struct_pack
    env.globals["get_size"] = java_get_size
    env.globals["get_schema"] = java_get_schema
    env.globals["get_protobuf_unpack"] = java_protobuf_unpack
    env.globals["get_protobuf_pack"] = java_protobuf_pack
    env.globals["get_nested"] = java_get_nested
    env.globals["assert_equals"] = java_assert_equals
    env.globals["test_proto_setter"] = test_proto_setter
    env.globals["strip_units"] = strip_units

    wpimath_dir = "/home/pjreiniger/git/allwpilib/wpimath"
    wpimath_src_dir = os.path.join(wpimath_dir, "src/main/java/edu/wpi/first/math")
    wpimath_test_dir = os.path.join(wpimath_dir, "src/test/java/edu/wpi/first/math")

    kwargs = dict(
                protobuf_type=protobuf_type,
                lang_type=lang_type,
                has_nested_types=has_nested_types,
                clazz=clazz,
                fields=clazz[1].DESCRIPTOR.fields,
                package=java_package,
                java_package=output_directory,
                protobuf_package=java_package,)

    proto_src = os.path.join(wpimath_src_dir, f"{output_directory}/serde/{lang_type}ProtoSerde.java")
    with open(proto_src, "w") as f:
        f.write(env.get_template("java_proto_serde.jinja2").render(**kwargs))

    struct_src = os.path.join(wpimath_src_dir, f"{output_directory}/serde/{lang_type}StructSerde.java")
    with open(struct_src, "w") as f:
        f.write(env.get_template("java_struct_serde.jinja2").render(**kwargs))


    proto_test = os.path.join(wpimath_test_dir, f"{output_directory}/serde/{lang_type}SerdeTest.java")
    with open(proto_test, "w") as f:
        f.write(env.get_template("java_serde_test.jinja2").render(**kwargs))

    # struct_test = os.path.join(wpimath_test_dir, f"{output_directory}/{lang_type}StructSerdeTest.java")
    # with open(struct_test, "w") as f:
    #     f.write(env.get_template("java_struct_test.jinja2").render(**kwargs))
