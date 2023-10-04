
#include "wpi/json.h"

#define BASIC_JSON_SPECIALIZATION basic_json<std::map, std::vector, std::string, bool, std::int64_t, std::uint64_t, double, std::allocator, adl_serializer, std::vector<std::uint8_t>>


WPI_JSON_NAMESPACE_BEGIN


#define BASIC_JSON_SPECIALIZATION basic_json<std::map, std::vector, std::string, bool, std::int64_t, std::uint64_t, double, std::allocator, adl_serializer, std::vector<std::uint8_t>>

// json.h
template class BASIC_JSON_SPECIALIZATION;
template class json_sax<BASIC_JSON_SPECIALIZATION>;

// json_pointer.h
template class json_pointer<std::string>;

namespace detail
{
////////////////////////
// detail/conversions
////////////////////////
    
// detail/conversions/from_json.h
template void from_json(const BASIC_JSON_SPECIALIZATION& j, std::nullptr_t& n);
template void from_json(const BASIC_JSON_SPECIALIZATION& j, bool& n);
template void from_json(const BASIC_JSON_SPECIALIZATION& j, std::string& n);
template void from_json(const BASIC_JSON_SPECIALIZATION& j, double& n);
template void from_json(const BASIC_JSON_SPECIALIZATION& j, std::int64_t& n);
template void from_json(const BASIC_JSON_SPECIALIZATION& j, std::uint64_t& n);
template void from_json(const BASIC_JSON_SPECIALIZATION& j, std::vector<uint8_t>& n);

// detail/conversions/to_chars.h
namespace dtoa_impl {
template boundaries compute_boundaries(float value);
template boundaries compute_boundaries(double value);

template void grisu2(char* buf, int& len, int& decimal_exponent, float value);
template void grisu2(char* buf, int& len, int& decimal_exponent, double value);
}
template char* to_chars(char* first, const char* last, float value);
template char* to_chars(char* first, const char* last, double value);

// detail/conversions/to_json.h
template void external_constructor<value_t::boolean>::construct(BASIC_JSON_SPECIALIZATION& j, const bool);
template void external_constructor<value_t::string>::construct(BASIC_JSON_SPECIALIZATION& j, const std::string&);
template void external_constructor<value_t::string>::construct(BASIC_JSON_SPECIALIZATION& j, std::string&&);
template void external_constructor<value_t::binary>::construct(BASIC_JSON_SPECIALIZATION& j, const BASIC_JSON_SPECIALIZATION::binary_t&);
template void external_constructor<value_t::binary>::construct(BASIC_JSON_SPECIALIZATION& j, BASIC_JSON_SPECIALIZATION::binary_t&&);
template void external_constructor<value_t::number_float>::construct(BASIC_JSON_SPECIALIZATION& j, double);
template void external_constructor<value_t::number_unsigned>::construct(BASIC_JSON_SPECIALIZATION& j, std::uint64_t);
template void external_constructor<value_t::number_integer>::construct(BASIC_JSON_SPECIALIZATION& j, std::int64_t);
template void external_constructor<value_t::array>::construct(BASIC_JSON_SPECIALIZATION& j, const std::vector<BASIC_JSON_SPECIALIZATION>&);
template void external_constructor<value_t::array>::construct(BASIC_JSON_SPECIALIZATION& j, std::vector<BASIC_JSON_SPECIALIZATION>&&);
template void external_constructor<value_t::object>::construct(BASIC_JSON_SPECIALIZATION& j, const BASIC_JSON_SPECIALIZATION::object_t&);
template void external_constructor<value_t::object>::construct(BASIC_JSON_SPECIALIZATION& j, BASIC_JSON_SPECIALIZATION::object_t&&);

template void to_json(BASIC_JSON_SPECIALIZATION& j, BASIC_JSON_SPECIALIZATION::string_t&&);
template void to_json(BASIC_JSON_SPECIALIZATION& j, const BASIC_JSON_SPECIALIZATION::binary_t&);
template void to_json(BASIC_JSON_SPECIALIZATION& j, BASIC_JSON_SPECIALIZATION::array_t&&);
template void to_json(BASIC_JSON_SPECIALIZATION& j, BASIC_JSON_SPECIALIZATION::object_t&&);


////////////////////////
// detail/input
////////////////////////
// detail/input/binary_reader.h
template class binary_reader<BASIC_JSON_SPECIALIZATION, file_input_adapter, json_sax_dom_parser<BASIC_JSON_SPECIALIZATION>>;
template class binary_reader<BASIC_JSON_SPECIALIZATION, file_input_adapter, json_sax_dom_callback_parser<BASIC_JSON_SPECIALIZATION>>;
template class binary_reader<BASIC_JSON_SPECIALIZATION, file_input_adapter, json_sax_acceptor<BASIC_JSON_SPECIALIZATION>>;

template class binary_reader<BASIC_JSON_SPECIALIZATION, input_stream_adapter, json_sax_dom_parser<BASIC_JSON_SPECIALIZATION>>;
template class binary_reader<BASIC_JSON_SPECIALIZATION, input_stream_adapter, json_sax_dom_callback_parser<BASIC_JSON_SPECIALIZATION>>;
template class binary_reader<BASIC_JSON_SPECIALIZATION, input_stream_adapter, json_sax_acceptor<BASIC_JSON_SPECIALIZATION>>;


// detail/input/json_sax.h
template class json_sax_dom_parser<BASIC_JSON_SPECIALIZATION>;
template class json_sax_dom_callback_parser<BASIC_JSON_SPECIALIZATION>;
template class json_sax_acceptor<BASIC_JSON_SPECIALIZATION>;


////////////////////////
// detail/output
////////////////////////
// detail/output/binary_writer.h
template class binary_writer<BASIC_JSON_SPECIALIZATION, char>;
template class binary_writer<BASIC_JSON_SPECIALIZATION, std::uint8_t>;

// detail/output/serializer.h
template class serializer<BASIC_JSON_SPECIALIZATION>;

////////////////////////
// detail
////////////////////////
// detail/hash.h
template std::size_t hash(const BASIC_JSON_SPECIALIZATION& j);

}

WPI_JSON_NAMESPACE_END