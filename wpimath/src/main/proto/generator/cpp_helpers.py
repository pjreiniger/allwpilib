from google.protobuf.descriptor import FieldDescriptor
from jinja2 import Environment, FileSystemLoader
from wpimath.src.main.proto.generator.lib import (
    MessageClass,
    ProtobufModule,
    MessageField,
    lower_camel_case,
    render_template,
)
import os

PROTO_TYPE_TO_CPP_TEXT = {
    FieldDescriptor.TYPE_DOUBLE: "double",
    FieldDescriptor.TYPE_BOOL: "boolean",
    FieldDescriptor.TYPE_INT32: "int",
    FieldDescriptor.TYPE_UINT32: "int",
}


def __get_schema(message: MessageClass):
    return ";".join(
        field.get_schema(PROTO_TYPE_TO_CPP_TEXT) for field in message.fields
    )


def __get_struct_size(message):
    primitive_size = 0
    subclass_parts = []

    output = ""

    for field in message.fields:
        if field.is_message:
            subclass_parts.append(f"wpi::Struct<frc::{field.message_type}>::kSize")
        elif field.protobuf_type == FieldDescriptor.TYPE_DOUBLE:
            primitive_size += 8
        elif field.protobuf_type == FieldDescriptor.TYPE_INT32:
            num_doubles += 1
        elif field.protobuf_type == FieldDescriptor.TYPE_UINT32:
            num_doubles += 1
        else:
            raise Exception(f"Unsupported type {field.type} for field {field.name}")

    if primitive_size > 0:
        output += f"{primitive_size}"

    if subclass_parts and output:
        output += "+ "
    output += " + ".join(subclass_parts)

    return output


def __maybe_wrap_with_unit(field: MessageField, txt):
    if field.name.endswith("_radians"):
        return "units::radian_t{" + txt + "}"
    elif field.name.endswith("_meters"):
        return "units::meter_t{" + txt + "}"
    elif field.name.endswith("_volts"):
        return "units::volt_t{" + txt + "}"
    elif field.name.endswith("_mps"):
        return "units::meters_per_second_t{" + txt + "}"
    elif field.name.endswith("_rps"):
        return "units::radians_per_second_t{" + txt + "}"

    print("No units for ", field.name)

    return txt


def __struct_unpack(field: MessageField):
    if field.is_message:
        return f"wpi::UnpackStruct<frc::{field.message_type}, 0>(data)"

    output = (
        f"wpi::UnpackStruct<{PROTO_TYPE_TO_CPP_TEXT[field.protobuf_type]}, 0>(data)"
    )
    output = __maybe_wrap_with_unit(field, output)

    return output


def __struct_pack(field: MessageField):
    if field.is_message:
        return f"wpi::PackStruct<0>(data, value.{__local_getter(field)})"

    output = f"wpi::PackStruct<0>(data, value.{__local_getter(field)})"

    return output


def __proto_getter(field: MessageField):
    return f"{field.name}"


def __proto_setter(field: MessageField):
    if field.is_message:
        return f"mutable_{field.name}()->"
    return f"set_{field.name}"


def __local_getter(field: MessageField):
    field_name = field.name_without_units
    field_name = field_name.replace("meters", "")
    field_name = field_name.replace("volts", "")
    # field_name = field_name.replace("mps", "meters_per_second")
    # field_name = field_name.replace("rps", "radians_per_second")

    # return upper_camel_case(field_name) + "()"
    # return upper_camel_case(field_name) + "().value()"
    # return upper_camel_case(field_name) + ".value()"
    return lower_camel_case(field_name) + ".value()"


def __proto_unpack(field: MessageField):
    if field.is_message:
        return f"wpi::UnpackProtobuf<frc::{field.message_type}>(m->{__proto_getter(field)}())"

    output = f"m->{__proto_getter(field)}()"
    output = __maybe_wrap_with_unit(field, output)
    return output


def __proto_pack(field: MessageField):
    if field.is_message:
        return f"wpi::PackProtobuf(m->mutable_{__proto_getter(field)}(), value.{__local_getter(field)})"

    output = f"m->{__proto_setter(field)}(value.{__local_getter(field)})"
    return output


def __test_proto_setter(field: MessageField):
    return f"proto.{__proto_setter(field)}(kExpectedData.{__local_getter(field)});"


def __assert_local_equals(field: MessageField):
    return f"EXPECT_EQ(kExpectedData.{__local_getter(field)}, unpacked_data.{__local_getter(field)});"


def __assert_local_vs_proto_equals(field: MessageField):
    return f"EXPECT_EQ(kExpectedData.{__local_getter(field)}, proto.{__proto_getter(field)}());"


def render_message_cpp(
    module: ProtobufModule, message: MessageClass, force_tests: bool
):
    env = Environment(
        loader=FileSystemLoader(
            "/home/pjreiniger/git/allwpilib/wpimath/src/main/proto/generator/templates"
        )
    )
    env.globals["get_size"] = __get_struct_size
    env.globals["get_schema"] = __get_schema
    env.globals["struct_unpack"] = __struct_unpack
    env.globals["struct_pack"] = __struct_pack
    env.globals["proto_unpack"] = __proto_unpack
    env.globals["proto_pack"] = __proto_pack

    env.globals["test_proto_setter"] = __test_proto_setter
    env.globals["assert_local_vs_proto_equals"] = __assert_local_vs_proto_equals
    env.globals["assert_local_equals"] = __assert_local_equals

    kwargs = dict(
        module=module,
        message=message,
    )

    lang_type = message.local_type

    wpimath_dir = "/home/pjreiniger/git/allwpilib/wpimath"

    # wpimath_test_dir = os.path.join(wpimath_dir, "src/test/java/edu/wpi/first/math")

    wpimath_incl_dir = os.path.join(wpimath_dir, "src/main/native/include/frc")
    wpimath_incl_serde_dir = os.path.join(wpimath_incl_dir, module.subfolder, "serde")
    serde_hdr = os.path.join(wpimath_incl_serde_dir, f"{lang_type}Serde.inc")
    # render_template(env, "cpp_serde.h.jinja2", serde_hdr, **kwargs)

    wpimath_cpp_dir = os.path.join(wpimath_dir, "src/main/native/cpp/")
    wpimath_cpp_serde_dir = os.path.join(wpimath_cpp_dir, module.subfolder, "serde")
    serde_cpp = os.path.join(wpimath_cpp_serde_dir, f"{lang_type}Serde.cpp")
    # render_template(env, "cpp_serde.cpp.jinja2", serde_cpp, **kwargs)

    wpimath_test_dir = os.path.join(wpimath_dir, "src/test/native/cpp/")
    wpimath_test_serde_dir = os.path.join(wpimath_test_dir, module.subfolder, "serde")
    serde_test = os.path.join(wpimath_test_serde_dir, f"{lang_type}SerdeTest.cpp")
    if force_tests or not os.path.exists(serde_test):
        render_template(env, "cpp_test.jinja2", serde_test, **kwargs)
