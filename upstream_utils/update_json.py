#!/usr/bin/env python3

import os
import re
import shutil
import subprocess

from upstream_utils import (
    get_repo_root,
    clone_repo,
    walk_cwd_and_copy_if,
    git_am,
)



def my_log(*kargs, **kwargs):
    print(*kargs, **kwargs)


def run_basic_json_replacement(all_files):
    for wpi_file in all_files:

        with open(wpi_file) as f:
            content = f.read()

        content = content.replace("binary_reader::BasicJsonType", "json")
        content = content.replace("binary_writer::BasicJsonType", "json")
        content = content.replace("json_pointer::json", "json")
        content = content.replace("parser::json", "json")

        for x in ["json_parser", "json_pointer", "json_serializer", "binary_writer", "json_sax"]:
            if x in wpi_file:
                content = content.replace("BasicJsonType", "json")
                content = content.replace("BasicJsonType", "json")
        content = content.replace("json_pointer::json", "json")
        content = content.replace("json::json ", "json ")
        content = content.replace("json::json&", "json&")

        with open(wpi_file, "w") as f:
            f.write(content)

def run_global_replacements(wpi_files):
    # return
    for wpi_file in wpi_files:
        with open(wpi_file) as f:
            content = f.read()

        class TypeReplacement:
            def __init__(self, their_type, standard_type, comment_type=None):
                self.their_type = their_type
                self.standard_type = standard_type
                self.comment_type = comment_type or self.standard_type

        types_to_replace = []
        types_to_replace.append(TypeReplacement("number_integer_t", "int64_t"))
        types_to_replace.append(TypeReplacement("number_unsigned_t", "uint64_t"))
        types_to_replace.append(TypeReplacement("number_float_t", "double"))
        types_to_replace.append(TypeReplacement("boolean_t", "bool"))
        types_to_replace.append(TypeReplacement("string_t", "std::string", "`std::string`"))

        for tr in types_to_replace:
            content = re.sub(f" +using {tr.their_type} = typename BasicJsonType::{tr.their_type};\n", "", content)
            content = re.sub(f" +using other_{tr.their_type} = typename BasicJsonType::{tr.their_type};\n", "", content)
            content = content.replace(f"typename {tr.their_type}::value_type", f"{tr.standard_type}::value_type")
            content = content.replace(f"typename BasicJsonType::{tr.their_type}", f"{tr.standard_type}")
            content = content.replace(f"@ref basic_json::{tr.their_type}", tr.standard_type)
            content = content.replace(f"@ref {tr.their_type}", tr.comment_type)
            content = re.sub(f"\\b{tr.their_type}\\b", tr.standard_type, content)
        #     content = content.replace(f"{tr.their_type}", tr.standard_type)

        content = content.replace("basic_json", "json")
        content = content.replace("::nlohmann", "::wpi")
        content = content.replace("nlohmann::", "wpi::")
        content = content.replace("    BasicJsonType ", "    json ")
        content = content.replace("JSONSerializer<ValueTypeCV>", "adl_serializer<ValueTypeCV, void>")
        content = content.replace("JSONSerializer<ValueType>", "adl_serializer<ValueType, void>")
        content = content.replace("JSONSerializer<U>", "adl_serializer<U, void>")
        content = content.replace("JSONSerializer<T, SFINAE>", "adl_serializer<T, SFINAE>")
        content = content.replace("const typename object_t::key_type& key", "std::string_view key")

        content = re.sub(r'type_error::create\(([0-9]+), (".*?) " \+ std::string\(type_name\(\)\)', r'type_error::create(\1, \2", type_name()', content)
        content = re.sub(r'type_error::create\(([0-9]+), (".*?) " \+ std::string\((.*).type_name\(\)\)', r'type_error::create(\1, \2", \3.type_name()', content)
        # content = re.sub(r'type_error::create\(([0-9]+), ".*?" + std::string\(type_name\(\)\)', 'HELLO', content)

        content = re.sub("oa->write_character\((.*)\)", r"o << \1", content)
        content = re.sub("o->write_character\((.*)\)", r"o << \1", content)
        content = re.sub("o->write_characters\((.*)\)", r"o.write(\1)", content)
        content = re.sub("o\.write\(\"(.*)\", [0-9]+\);", r'o << "\1";', content)
        content = re.sub(r"dump_escaped\(i\-\>first\, ensure_ascii\)\;", "dump_escaped((*i)->first(), ensure_ascii);", content)
        content = re.sub(r"dump\(i\-\>second\, false\, ensure_ascii\, indent_step\, current_indent\);", "dump((*i)->second, false, ensure_ascii, indent_step, current_indent);", content)
        content = re.sub(r"dump\(i\-\>second\, true\, ensure_ascii\, indent_step\, new_indent\);", "dump((*i)->second, true, ensure_ascii, indent_step, new_indent);", content)
        # content = re.sub("o->write\(\"(.*)\", [0-9]+\)", r"o.write(\1)", content)

        content = re.sub(r"""                        string_buffer\[bytes\+\+\] = '(.*)';
                        string_buffer\[bytes\+\+\] = '(.*)';""",
                         r"""                        o << '\1' << '\2';""",
                         content, re.MULTILINE)


        content = re.sub("AllocatorType<(.*?)>", r"std::allocator<\1>", content)

        content = content.replace("const std::string& what_arg", "std::string_view what_arg")
        content = content.replace("const char* what_arg", "std::string_view what_arg")
        content = content.replace("exception::exception(int exception::id_, std::string_view what_arg)", "exception::exception(int id_, std::string_view what_arg)")

        content = content.replace("namespace nlohmann", "namespace wpi")
        content = content.replace("json::lexer::std::char_traits<char>::int_type json::lexer::get()", "std::char_traits<char>::int_type json::lexer::get()")

        content = content.replace("""json::binary_writer::template<typename json::binary_writer::NumberType>
    void write_number(const NumberType n)""", """template<typename NumberType>
void json::binary_writer::write_number(const NumberType n)""")

        content = content.replace("""json::binary_writer::template<typename json::binary_writer::NumberType, typename std::enable_if<
                 std::is_floating_point<NumberType>::value, int>::type>
    void write_number_with_ubjson_prefix(const NumberType n,
                                         const bool add_prefix)""", """template<typename NumberType, typename std::enable_if<
                 std::is_floating_point<NumberType>::value, int>::type>
void json::binary_writer::write_number_with_ubjson_prefix(const NumberType n,
                                         const bool add_prefix)""")

        content = content.replace("""json::binary_writer::template<typename json::binary_writer::NumberType, typename std::enable_if<
                 std::is_unsigned<NumberType>::value, int>::type>
    void write_number_with_ubjson_prefix(const NumberType n,
                                         const bool add_prefix)""", """template<typename NumberType, typename std::enable_if<
                 std::is_unsigned<NumberType>::value, int>::type>
void json::binary_writer::write_number_with_ubjson_prefix(const NumberType n,
                                         const bool add_prefix)""")

        content = content.replace("""json::binary_writer::template<typename json::binary_writer::NumberType, typename std::enable_if<
                 std::is_signed<NumberType>::value and
                 not std::is_floating_point<NumberType>::value, int>::type>
    void write_number_with_ubjson_prefix(const NumberType n,
                                         const bool add_prefix)""", """template<typename NumberType, typename std::enable_if<
                 std::is_signed<NumberType>::value and
                 not std::is_floating_point<NumberType>::value, int>::type>
void json::binary_writer::write_number_with_ubjson_prefix(const NumberType n,
                                         const bool add_prefix)""")

        content = content.replace("json::json(json&& json::other) noexcept", "json::json(json&& other) noexcept")
        content = content.replace('#include "/json_wpi/json.h"', '#include "wpi/json.h"')
        content = content.replace("json::bool", "bool")
        content = content.replace("""json::template json::<typename SAX>
    bool sax_parse(raw_istream& is, SAX* sax,
                          input_format_t format = input_format_t::json,
                          const bool strict)""", """template <typename SAX>
bool json::sax_parse(raw_istream& is, SAX* sax,
                      input_format_t format,
                      const bool strict)""")

        content = content.replace("""json::binary_reader::template<typename json::binary_reader::NumberType>
    bool get_number(NumberType& result)""", """template<typename NumberType>
bool json::binary_reader::get_number(NumberType& result)""")

        content = content.replace("""json::binary_reader::template<typename json::binary_reader::NumberType>
    bool get_string(const NumberType len, std::string& result)""", """template<typename NumberType>
bool json::binary_reader::get_string(const NumberType len, std::string& result)""")

        content = content.replace("""json::parser::template json::parser::<typename SAX>
    bool sax_parse_internal(SAX* sax)""", """template <typename SAX>
bool json::parser::sax_parse_internal(SAX* sax)""")
        content = content.replace("void json::parser::parse(const bool strict, json::parser::BasicJsonType& result)", "void json::parser::parse(const bool strict, json& result)")

        content = content.replace("void json::serializer::dump(const json::serializer::BasicJsonType& val, const bool pretty_print,", "void json::serializer::dump(const json& val, const bool pretty_print,")

        # Fix exception throwing
        content = re.sub(
            """(\s+?)std::stringstream ss;
\s+ss << std::setw\(2\) << std::uppercase << std::setfill\('0'\) << std::hex << current;
\s+JSON_THROW\((.*?)::create\(([0-9]+), chars_read,(\s+)\"(.*) last byte: 0x" \+ ss.str\(\)\)\)""",

                         r"""\1JSON_THROW(\2::create(\3, chars_read,\4fmt::format("\5 last byte: {:#02x}", current)))""",
                         content, re.MULTILINE)

        content = re.sub(
            """(\s+?)JSON_THROW\((.*?)::create\(([0-9]+),(\s+)"(.*?)" \+ std::to_string\((.*?)\)\)\)""",
            r"""\1JSON_THROW(\2::create(\3,\4fmt::format("\5{}", \6)))""",
                         content)

        # content = re.sub(
        #     """(.*?)::create\(([0-9]+),(\s+)"(.*?)" \+ std::to_string\((.*?)\)\)\)""",
        #     r"""\1JSON_THROW(\2::create(\3,\4fmt::format("\5{}",)))""",
        #     content)

        content = re.sub(r'"error reading (.*) last byte: 0x" \+ last_token', r'fmt::format("error reading \1 last byte: {:#02x}", current)', content)
        content = re.sub(r'"expected a (.*) last byte: 0x" \+ last_token', r'fmt::format("expected a \1 last byte: {:#02x}", current)', content)
        content = re.sub(r'"byte after (.*) last byte: 0x" \+ last_token', r'fmt::format("byte after \1 last byte: {:#02x}", current)', content)

        content = re.sub(
            """(\s+?)JSON_THROW\((.*?)::create\(([0-9]+),(\s+)"(.*?)" \+ std::to_string\((.*)\) \+ "(.*)"\)\)""",
            r'\1JSON_THROW(\2::create(\3,\4fmt::format("\5{}\7", \6)))',
            content)

        content = re.sub(
            """(\s+?)JSON_THROW\((.*?)::create\(([0-9]+),(\s+)"(.*?)" \+ (.*) \+ "(.*)"\)\)""",
            r'\1JSON_THROW(\2::create(\3,\4fmt::format("\5{}\7", \6)))',
            content)

        content = re.sub(
            """(\s+?)JSON_THROW\((.*?)::create\(([0-9]+),(\s+)"(.*?)" \+ (.*)\)\)""",
            r'\1JSON_THROW(\2::create(\3,\4fmt::format("\5{}", \6)))',
            content)

        # content = re.sub(
        #     """(\s+?)JSON_THROW\((.*?)::create\(([0-9]+), ([0-9]+), "([a-zA-Z]?)" \+ (.*?) \+ "(.*?)"\)\);""",
        #     r'''\1JSON_THROW(\2::create(\3, \4, fmt::format("\5{}\7", \6)));''',
        #     content)
        #
        # content = re.sub(
        #     """(\s+?)JSON_THROW\((.*?)::create\(([0-9]+), ([0-9]+), (.*?) \+ "(.*)" \+ (.*) \+ (.*)\)\)""",
        #     r'\1JSON_THROW(\2::create(\3, \4, fmt::format("{}\6' + "{}'" + r'", \5, \7)))',
        #     content)


        # Fixup bad export names
        content = content.replace("json::lexer::lexer(raw_istream& json::lexer::s)", "json::lexer::lexer(raw_istream& s)")

        content = content.replace("json::json_value(value_t json::t)", "json::json_value::json_value(value_t t)")
        content = content.replace("json::json(initializer_list_t json::init,", "json::json(initializer_list_t init,")
        content = content.replace("json::json(size_type json::cnt, const json& val)", "json::json(size_type cnt, const json& val)")
        content = content.replace("json::json(const json::json& other)", "json::json(const json& other)")
        content = content.replace("bool json::operator==(const_reference lhs, const_reference rhs) noexcept", "bool operator==(json::const_reference lhs, json::const_reference rhs) noexcept")
        content = content.replace("bool json::operator<(const_reference lhs, const_reference rhs) noexcept", "bool operator<(json::const_reference lhs, json::const_reference rhs) noexcept")
        content = content.replace("json::raw_ostream& json::operator<<(raw_ostream& o, const json& j)", "raw_ostream& operator<<(raw_ostream& o, const json& j)")
        content = content.replace("json::std::ostream& json::operator<<(std::ostream& o, const json& j)", "std::ostream& operator<<(std::ostream& o, const json& j)")
        content = content.replace("::raw_istream& ::operator>>(raw_istream& i, json& j)", "raw_istream& operator>>(raw_istream& i, json& j)")
        content = content.replace("lexer::std::char_traits<char>::int_type lexer::get()", "std::char_traits<char>::int_type lexer::get()")
        content = content.replace("lexer::lexer(raw_istream& lexer::s)", "lexer::lexer(raw_istream& s)")
        content = content.replace("void parser::parse(const bool strict, parser::json& result)", "void parser::parse(const bool strict, json& result)")

        content = content.replace("""binary_reader::template<typename binary_reader::NumberType>
    std::string get_string(const NumberType len)""", """template<typename NumberType>
std::string binary_reader::get_string(const NumberType len)""")

        content = content.replace("""binary_reader::template<typename binary_reader::NumberType>
    json get_cbor_array(const NumberType len)""", """template<typename NumberType>
json fffffffbinary_reader::get_cbor_array(const NumberType len)""")


        for err in ["parse_error", "invalid_iterator", "type_error", "out_of_range", "other_error"]:
            content = content.replace(f"{err}::{err} {err}", f"{err} {err}")

        # Fixup includes
        # content = re.sub('#include <nlohmann/(.*)>', r'#include "wpi/\1"', content)


        with open(wpi_file, "w") as f:
            f.write(content)

def fixup_include_name(line):
    if not line.startswith("#include <nlohmann"):
        return line
    file_to_include_search = re.findall(r"\<(.*)\>", line)
    file_to_include = file_to_include_search[0]
    folders = file_to_include.strip()[:-2].split("/")
    output = f"#include \"{'/'.join(folders[:-1])}/"
    filename = folders[-1]
    if not filename.startswith("json"):
        filename = "json_" + filename

    output += filename + "\"\n"
    output = output.replace("nlohmann", "wpi")
    output = output.replace("\\", "/")
    return output


    # return contents


def extract_includes(lines, line_iter):
    line_iter += 1

    includes = ""
    while line_iter < len(lines):
        line = lines[line_iter]
        line_iter += 1

        if line.strip() == "//-------EXPORTED_INCLUDE_END":
            break

        includes += line

    return includes, line_iter

def extract_function(lines, line_iter):
    """
//-------EXPORTED_FUNC_DEF_START 
//-------EXPORTED_FUNC_DEF_END
//-------EXPORTED_FUNC_CONTENTS_START
//-------EXPORTED_FUNC_CONTENTS_END
"""
    MAGIC_START = "//-------EXPORTED"
    EXPORTED_FUNC_DEF_END = "//-------EXPORTED_FUNC_DEF_END"
    EXPORTED_FUNC_CONTENTS_START = "//-------EXPORTED_FUNC_CONTENTS_START"
    EXPORTED_FUNC_CONTENTS_END = "//-------EXPORTED_FUNC_CONTENTS_END"

    class_name = lines[line_iter][32:].strip()
    line_iter += 1

    hpp_contents = ""
    cpp_contents = ""

    def strip_indent(line, indent_size):
        if len(line) > indent_size and line[:indent_size].strip() == "":
            line = line[indent_size:] # Strip the indent
        return line

    function_def = ""
    indent_size = None
    while line_iter < len(lines):
        line = lines[line_iter]
        line_iter += 1

        if line.startswith(MAGIC_START) and not line.startswith(EXPORTED_FUNC_DEF_END):
            raise

        if line.startswith(EXPORTED_FUNC_DEF_END):
            hpp_contents = hpp_contents[:-1] + ";\n"
            break
        
        hpp_contents += line
        indent_size = indent_size or len(line) - len(line.lstrip())
        function_def += line

    function_def = strip_indent(function_def, indent_size)

    function_def = function_def.replace("static ", "")
    function_def = function_def.replace("friend ", "")
    function_def = function_def.replace("explicit ", "")
    function_def = function_def.replace("[[noreturn]] ", "")
    function_def = function_def.replace(" = true", "")
    function_def = function_def.replace(" = false", "")
    function_def = function_def.replace(" = -1", "")
    function_def = function_def.replace(" = nullptr", "")
    function_def = function_def.replace(" = value_t::array", "")
    function_def = function_def.replace(" = ' '", "")
    function_def = function_def.replace(" = 0", "")
    function_def = function_def.replace(" = \"\"", "")
    function_def_parts = function_def.strip().split(" ")
    ret_type = function_def_parts[0]

    # if "parse_cbor_internal" in function_def:
    #     print(function_def)
    #     print("----", ret_type)
    #     # raise
    #
    # if "string" in ret_type:
    #     print("Hello", ret_type)

    def basic_function_def():
        return "\n" + ret_type + " " + function_def_parts[1] + f" {class_name}::" + " ".join(function_def_parts[2:])

    def fundef2():
        return "\n" + ret_type + f" {class_name}::" + " ".join(function_def_parts[1:])

    function_definition = None

    if ret_type == "friend":
        cpp_contents += "------" + "\n" + ret_type + f" {class_name}::" + " ".join(function_def_parts[1:])
    elif ret_type in ["int",
                      "void",
                      "bool",
                      "std::string",
                      "string_t",
                      "std::span<uint8_t>",
                      "uint8_t"]:
        function_definition =  fundef2()
    elif ret_type == "const" and function_def_parts[1] == "char*":
        function_definition =  basic_function_def()
    elif ret_type == "const" and (function_def_parts[1] == "BasicJsonType&" or function_def_parts[1] == "json&"):
        function_definition =  basic_function_def()
    elif "std::pair" in ret_type:
        function_definition =  basic_function_def()
    elif "std::vector" in ret_type:
        function_definition =  fundef2()
    elif ret_type in ["BasicJsonType&", "BasicJsonType"]:
        function_definition = fundef2()
    elif ret_type in ["basic_json", "json", "json&", "BasicJsonType&", "BasicJsonType"]:
        function_definition =  fundef2()

    # elif ret_type == "json":
    #     function_definition =  "------" + "\n" + ret_type + f" {class_name}::" + " ".join(function_def_parts[1:])
    elif ret_type in [  "json",
                        "basic_json",
                        "token_type",
                        "iterator",
                        "size_type",
                        "reference",
                        "string_t",
                        "parser_error",
                        "parse_error",
                        "type_error",
                        "out_of_range",
                        "other_error",
                        "invalid_iterator",
                        "const_reference",
                        # "BasicJsonType&",
                        # "BasicJsonType",
                        ]:
        ret_type = class_name + "::" + ret_type
        function_definition =  fundef2()
    else:
        ret_type = class_name + "::" + ret_type
        function_definition =  fundef2()


    function_definition = function_definition.replace("BasicJsonType", f"{class_name}::BasicJsonType")
    cpp_contents += function_definition + "\n"

    while line_iter < len(lines):
        line = lines[line_iter]
        line_iter += 1

        if line.startswith(MAGIC_START) and not line.startswith(EXPORTED_FUNC_CONTENTS_START):
            raise
        if line.startswith(EXPORTED_FUNC_CONTENTS_START):
            break

    while line_iter < len(lines):
        line = lines[line_iter]
        line_iter += 1

        if line.startswith(MAGIC_START) and not line.startswith(EXPORTED_FUNC_CONTENTS_END):
            my_log(cpp_contents)
            raise
        if line.startswith(EXPORTED_FUNC_CONTENTS_END):
            break
            
        cpp_contents += strip_indent(line, indent_size)

    return hpp_contents, cpp_contents, line_iter


def handle_file(filename):
    with open(filename, 'r') as f:
        lines = f.readlines()

    hpp_contents = ""
    cpp_contents = ""
    exported_includes = ""

    line_iter = 0

    while line_iter < len(lines):
        line = lines[line_iter]
        line_iter += 1
        if line.startswith("//-------EXPORTED_FUNC") and not line.startswith("//-------EXPORTED_FUNC_DEF_START"):
            print(hpp_contents)
            raise
        if line.startswith("//-------EXPORTED_FUNC_DEF_START"):
            func_hpp_contents, func_cpp_contents, line_iter = extract_function(lines, line_iter - 1)
            hpp_contents += func_hpp_contents
            cpp_contents += func_cpp_contents
        elif line.startswith("//-------EXPORTED_INCLUDE_START"):
            these_exported_includes, line_iter = extract_includes(lines, line_iter - 1)
            exported_includes += these_exported_includes
        elif line.startswith("#include"):
            line = fixup_include_name(line)
            hpp_contents += line
        else:
            hpp_contents += line

    return hpp_contents, cpp_contents, exported_includes

        
def copy_and_split_files(json_cpp_root, json_incl_root):
    split_files = []

    split_files.append(r'include\nlohmann/adl_serializer.hpp')
    split_files.append(r'include\nlohmann\json.hpp')
    split_files.append(r'include\nlohmann/json_fwd.hpp')

    split_files.append(r'include\nlohmann/detail/exceptions.hpp')
    split_files.append(r'include\nlohmann/detail/json_pointer.hpp')
    split_files.append(r'include\nlohmann/detail/json_ref.hpp')
    split_files.append(r'include\nlohmann/detail/macro_scope.hpp')
    split_files.append(r'include\nlohmann/detail/macro_unscope.hpp')
    split_files.append(r'include\nlohmann/detail/meta/cpp_future.hpp')
    split_files.append(r'include\nlohmann/detail/meta/detected.hpp')
    split_files.append(r'include\nlohmann/detail/meta/detected.hpp')
    split_files.append(r'include\nlohmann/detail/meta/is_sax.hpp')
    split_files.append(r'include\nlohmann/detail/meta/type_traits.hpp')
    split_files.append(r'include\nlohmann/detail/meta/void_t.hpp')

    split_files.append(r'include\nlohmann/detail/value_t.hpp')

    split_files.append(r'include\nlohmann/detail/conversions/from_json.hpp')
    split_files.append(r'include\nlohmann/detail/conversions/to_chars.hpp')
    split_files.append(r'include\nlohmann/detail/conversions/to_json.hpp')

    split_files.append(r'include\nlohmann\detail\input/binary_reader.hpp')
    split_files.append(r'include\nlohmann\detail\input/parser.hpp')
    split_files.append(r'include\nlohmann\detail\input/lexer.hpp')
    split_files.append(r'include\nlohmann\detail\input/json_sax.hpp')

    split_files.append(r'include\nlohmann/detail/iterators/primitive_iterator.hpp')
    split_files.append(r'include\nlohmann/detail/iterators/internal_iterator.hpp')
    split_files.append(r'include\nlohmann/detail/iterators/iter_impl.hpp')
    split_files.append(r'include\nlohmann/detail/iterators/iteration_proxy.hpp')
    split_files.append(r'include\nlohmann/detail/iterators/json_reverse_iterator.hpp')

    split_files.append(r'include\nlohmann\detail\output/binary_writer.hpp')
    split_files.append(r'include\nlohmann\detail\output/serializer.hpp')
    
    all_files = []
    
    for filename in split_files:
        relative_path = filename[len("include/nlohmann/"):-2]
        header_dir = os.path.join(json_incl_root, os.path.dirname(relative_path))
        if not os.path.exists(header_dir):
            os.makedirs(header_dir)

        base_name = os.path.basename(filename).split(".")[0]

        if not base_name.startswith("json_") and base_name != "json":
            base_name = "json_" + base_name
            
        hpp_contents, cpp_contents, exported_includes = handle_file(filename)

        def is_detail_namespace(filename):
            if "detail" not in filename:
                return False

            for x in ["binary_reader.hpp", "binary_writer.hpp", "parser.hpp", "pointer.hpp", "lexer.hpp", "serializer.hpp"]:
                if filename.endswith(x):
                    return False

            return True

        if cpp_contents.strip():
            cpp_filename = os.path.join(json_cpp_root, base_name + '.cpp' )
            with open(cpp_filename, 'w') as f:
                include_filename = f"#include <{filename[8:]}>"
                include_filename = fixup_include_name(include_filename)
                f.write(f'#define WPI_JSON_IMPLEMENTATION\n{include_filename}\n')
                if exported_includes:
                    f.write(exported_includes + "\n")
                f.write('namespace wpi\n{\n')
                if is_detail_namespace(filename):
                    f.write("namespace detail\n{\n")
                f.write(cpp_contents)
                if is_detail_namespace(filename):
                    f.write("}  // namespace detail\n")
                f.write("}  // namespace wpi\n")
                f.write("#undef WPI_JSON_IMPLEMENTATION\n")
            all_files.append(cpp_filename)
            
        hpp_filename = os.path.join(json_incl_root, header_dir, base_name + ".h")
        with open(hpp_filename, 'w') as f:
            # f.write(JSON_LICENSE.format(version=JSON_VERSION))
            f.write(hpp_contents)
        all_files.append(hpp_filename)

        
    return all_files

        # wpi_file = os.path.join(json_cpp_root, base_name + '.cpp' )
        # shutil.copy(f, wpi_file)
        # resolve_includes(wpi_file, includable_files)

        # wpi_file = os.path.join(json_incl_root, base_name + '.h' )
        # shutil.copy(f, wpi_file)
        # resolve_includes(wpi_file, includable_files)

def main():
    upstream_root = clone_repo("https://github.com/nlohmann/json.git", JSON_VERSION)
    wpilib_root = get_repo_root()
    wpiutil = os.path.join(wpilib_root, "wpiutil")

    json_cpp_root = os.path.join(wpiutil, "src/main/native/thirdparty/json/cpp")
    json_incl_root = os.path.join(wpiutil, "src/main/native/thirdparty/json/include/wpi")

    # Delete old install
    shutil.rmtree(os.path.join(wpiutil, "src/main/native/thirdparty/json"), ignore_errors=True)

    # Re-make the root dirs
    os.makedirs(json_cpp_root)
    os.makedirs(json_incl_root)

    # Apply patches to upstream Git repo
    os.chdir(upstream_root)

    for f in [
        # "0001-Export-functions-to-cpp-files.patch",
        # "0002-De-templatize-classes.patch",
        # "0003-Use-LLVM-stream-operators-string_view-for-keys.patch",
        # "0004-Remove-deprecated.patch",
        # "0005-Update-comments.patch",
        # "0006-Improved-floating-point-handling.patch",
        # "0007-Use-C-17-type-traits.patch",
        # "0008-Update-exceptions.patch",
        # "0009-Fixup-includes.patch",
        # "0010-Various-fixes.patch",
    ]:
        git_am(
            os.path.join(wpilib_root, "upstream_utils/json_patches", f),
            use_threeway=True,
            ignore_whitespace=True,
        )

    all_files = []

    all_files.extend(copy_and_split_files(json_cpp_root, json_incl_root))

    run_global_replacements(all_files)
    run_basic_json_replacement(all_files)
    

JSON_VERSION = "3.3.0"


JSON_LICENSE = """/*----------------------------------------------------------------------------*/
/* Modifications Copyright (c) 2017-2018 FIRST. All Rights Reserved.          */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/
/*
    __ _____ _____ _____
 __|  |   __|     |   | |  JSON for Modern C++
|  |  |__   |  |  | | | |  version {version}
|_____|_____|_____|_|___|  https://github.com/nlohmann/json

Licensed under the MIT License <http://opensource.org/licenses/MIT>.
Copyright (c) 2013-2018 Niels Lohmann <http://nlohmann.me>.

Permission is hereby  granted, free of charge, to any  person obtaining a copy
of this software and associated  documentation files (the "Software"), to deal
in the Software  without restriction, including without  limitation the rights
to  use, copy,  modify, merge,  publish, distribute,  sublicense, and/or  sell
copies  of  the Software,  and  to  permit persons  to  whom  the Software  is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE  IS PROVIDED "AS  IS", WITHOUT WARRANTY  OF ANY KIND,  EXPRESS OR
IMPLIED,  INCLUDING BUT  NOT  LIMITED TO  THE  WARRANTIES OF  MERCHANTABILITY,
FITNESS FOR  A PARTICULAR PURPOSE AND  NONINFRINGEMENT. IN NO EVENT  SHALL THE
AUTHORS  OR COPYRIGHT  HOLDERS  BE  LIABLE FOR  ANY  CLAIM,  DAMAGES OR  OTHER
LIABILITY, WHETHER IN AN ACTION OF  CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE  OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
"""


if __name__ == "__main__":
    main()
