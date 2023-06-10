#include "p3dcparser_composite1.cxx"
#include "p3dcparser_composite2.cxx"

#include "dcLexer.cxx.prebuilt"
#include "dcParser.cxx.prebuilt"

#include <emscripten/bind.h>

using namespace emscripten;

EMSCRIPTEN_BINDINGS(dcparser) {
  register_vector<unsigned char>("vector_uchar");

  enum_<DCPackType>("DCPackType")
  .value("PTInvalid", DCPackType::PT_invalid)

  .value("PTDouble", DCPackType::PT_double)
  .value("PTInt", DCPackType::PT_int)
  .value("PTUint", DCPackType::PT_uint)
  .value("PTInt64", DCPackType::PT_int64)
  .value("PTUInt64", DCPackType::PT_uint64)
  .value("PTString", DCPackType::PT_string)
  .value("PTBlob", DCPackType::PT_blob)

  .value("PTArray", DCPackType::PT_array)
  .value("PTField", DCPackType::PT_field)
  .value("PTClass", DCPackType::PT_class)
  .value("PTSwitch", DCPackType::PT_switch)
  ;

  enum_<DCSubatomicType>("DCSubatomicType")
  .value("STInt8", DCSubatomicType::ST_int8)
  .value("STInt16", DCSubatomicType::ST_int16)
  .value("STInt32", DCSubatomicType::ST_int32)
  .value("STInt64", DCSubatomicType::ST_int64)

  .value("STUint8", DCSubatomicType::ST_uint8)
  .value("STUint16", DCSubatomicType::ST_uint16)
  .value("STUint32", DCSubatomicType::ST_uint32)
  .value("STUint64", DCSubatomicType::ST_uint64)

  .value("STUint64", DCSubatomicType::ST_uint64)

  .value("STFloat64", DCSubatomicType::ST_float64)

  .value("STString", DCSubatomicType::ST_string)
  .value("STBlob", DCSubatomicType::ST_blob)
  .value("STBlob32", DCSubatomicType::ST_blob32)
  .value("STInt16array", DCSubatomicType::ST_int16array)
  .value("STInt32array", DCSubatomicType::ST_int32array)
  .value("STUint16array", DCSubatomicType::ST_uint16array)
  .value("STUint32array", DCSubatomicType::ST_uint32array)

  .value("STInt8array", DCSubatomicType::ST_int8array)
  .value("STUint8array", DCSubatomicType::ST_uint8array)

  .value("STUint32uint8array", DCSubatomicType::ST_uint32uint8array)

  .value("STChar", DCSubatomicType::ST_char)

  .value("STInvalid", DCSubatomicType::ST_invalid)
  ;

  class_<DCFile>("DCFile")
  .constructor<>()

  .function("clear", &DCFile::clear)
  .function("read", select_overload<bool(Filename)>(&DCFile::read))

  .property("numClasses", &DCFile::get_num_classes)
  .function("getClass", &DCFile::get_class, allow_raw_pointers())
  .function("getClassByName", &DCFile::get_class_by_name, allow_raw_pointers())
  .function("getSwitchByName", &DCFile::get_switch_by_name, allow_raw_pointers())

  .function("getFieldByIndex", &DCFile::get_field_by_index, allow_raw_pointers())

  .property("hash", &DCFile::get_hash)
  ;

  class_<DCPacker>("DCPacker")
  .constructor<>()

  .function("beginPack", &DCPacker::begin_pack, allow_raw_pointers())
  .function("endPack", &DCPacker::end_pack)

  .function("setUnpackData", select_overload<void(const vector_uchar &)>(&DCPacker::set_unpack_data))
  .function("beginUnpack", &DCPacker::begin_unpack, allow_raw_pointers())
  .function("endUnpack", &DCPacker::end_unpack)

  .function("push", &DCPacker::push)
  .function("pop", &DCPacker::pop)

  .function("packDouble", &DCPacker::pack_double)
  .function("packInt", &DCPacker::pack_int)
  .function("packUint", &DCPacker::pack_uint)
  .function("packInt64", &DCPacker::pack_int64)
  .function("packUint64", &DCPacker::pack_uint64)
  .function("packString", &DCPacker::pack_string)
  .function("packBlob", &DCPacker::pack_blob)
  .function("packLiteralValue", &DCPacker::pack_literal_value)
  .function("packDefaultValue", &DCPacker::pack_default_value)

  .function("unpackDouble", select_overload<double()>(&DCPacker::unpack_double))
  .function("unpackInt", select_overload<int()>(&DCPacker::unpack_int))
  .function("unpackUint", select_overload<unsigned int()>(&DCPacker::unpack_uint))
  .function("unpackInt64", select_overload<int64_t()>(&DCPacker::unpack_int64))
  .function("unpackUint64", select_overload<uint64_t()>(&DCPacker::unpack_uint64))
  .function("unpackString", select_overload<std::string()>(&DCPacker::unpack_string))
  .function("unpackBlob", select_overload<std::vector<unsigned char>()>(&DCPacker::unpack_blob))
  .function("unpackSkip", &DCPacker::unpack_skip)

  .function("rawUnpackInt8", select_overload<int()>(&DCPacker::raw_unpack_int8))
  .function("rawUnpackInt16", select_overload<int()>(&DCPacker::raw_unpack_int16))
  .function("rawUnpackInt32", select_overload<int()>(&DCPacker::raw_unpack_int32))
  .function("rawUnpackInt64", select_overload<int64_t()>(&DCPacker::raw_unpack_int64))

  .function("rawUnpackUint8", select_overload<unsigned int()>(&DCPacker::raw_unpack_uint8))
  .function("rawUnpackUint16", select_overload<unsigned int()>(&DCPacker::raw_unpack_uint16))
  .function("rawUnpackUint32", select_overload<unsigned int()>(&DCPacker::raw_unpack_uint32))
  .function("rawUnpackUint64", select_overload<uint64_t()>(&DCPacker::raw_unpack_uint64))

  .function("rawUnpackFloat64", select_overload<double()>(&DCPacker::raw_unpack_float64))

  .function("rawUnpackString", select_overload<std::string()>(&DCPacker::raw_unpack_string))
  .function("rawUnpackBlob", select_overload<std::vector<unsigned char>()>(&DCPacker::raw_unpack_blob))

  .property("data", &DCPacker::get_bytes)
  .property("packType", &DCPacker::get_pack_type)
  .property("moreNestedFields", &DCPacker::more_nested_fields)
  ;

  class_<DCDeclaration>("DCDeclaration")
  ;

  class_<DCClass, base<DCDeclaration>>("DCClass")
  // .constructor<DCFile *, const std::string, bool, bool>()
  // .property("dcFile", &DCClass::get_dc_file, allow_raw_pointers())
  .property("name", &DCClass::get_name)
  .property("number", &DCClass::get_number)

  .property("numParents", &DCClass::get_num_parents)
  .function("getParent", &DCClass::get_parent, allow_raw_pointers())

  .property("numFields", &DCClass::get_num_fields)
  .function("getField", &DCClass::get_field, allow_raw_pointers())
  .function("getFieldByName", &DCClass::get_field_by_name, allow_raw_pointers())
  .function("getFieldByIndex", &DCClass::get_field_by_index, allow_raw_pointers())

  .property("numInheritedFields", &DCClass::get_num_inherited_fields)
  .function("getInheritedField", &DCClass::get_inherited_field, allow_raw_pointers())
  ;

  class_<DCSwitch, base<DCDeclaration>>("DCSwitch")
  .property("name", &DCSwitch::get_name)
  .function("getKeyParameter", &DCSwitch::get_key_parameter, allow_raw_pointers())

  .property("numCases", &DCSwitch::get_num_cases)
  .function("getCase", &DCSwitch::get_case, allow_raw_pointers())
  .function("getDefaultCase", &DCSwitch::get_default_case, allow_raw_pointers())

  .function("getCaseByValue", &DCSwitch::get_case_by_value, allow_raw_pointers())
  .function("getNumFields", &DCSwitch::get_num_fields, allow_raw_pointers())
  .function("getField", &DCSwitch::get_field, allow_raw_pointers())
  .function("getFieldByName", &DCSwitch::get_field_by_name, allow_raw_pointers())
  ;

  class_<DCPackerInterface>("DCPackerInterface")
  .property("name", &DCPackerInterface::get_name)
  ;

  class_<DCKeywordList>("DCKeywordList")
  ;

  class_<DCField, base<DCPackerInterface>>("DCField")
  .property("number", &DCField::get_number)
  .function("getClass", &DCField::get_class, allow_raw_pointers())

  .function("validateRanges", &DCField::validate_ranges, allow_raw_pointers())

  // .function("hasKeyword", select_overload<bool(const std::string &)>(&DCField::has_keyword))

  .property("isRequired", &DCField::is_required)
  .property("isBroadcast", &DCField::is_broadcast)
  .property("isRam", &DCField::is_ram)
  .property("isDb", &DCField::is_db)
  .property("isClsend", &DCField::is_clsend)
  .property("isClrecv", &DCField::is_clrecv)
  .property("isOwnsend", &DCField::is_ownsend)
  .property("isOwnrecv", &DCField::is_ownrecv)
  .property("isAirecv", &DCField::is_airecv)

  .property("hasDefaultValue", &DCField::has_default_value)
  .function("getDefaultValue", &DCField::get_default_value)
  ;

  class_<DCParameter, base<DCField>>("DCParameter")
  .property("isValid", &DCParameter::is_valid)
  .function("asSimpleParameter", select_overload<DCSimpleParameter *()>(&DCParameter::as_simple_parameter), allow_raw_pointers())
  .function("asClassParameter", select_overload<DCClassParameter *()>(&DCParameter::as_class_parameter), allow_raw_pointers())
  .function("asSwitchParameter", select_overload<DCSwitchParameter *()>(&DCParameter::as_switch_parameter), allow_raw_pointers())
  .function("asArrayParameter", select_overload<DCArrayParameter *()>(&DCParameter::as_array_parameter), allow_raw_pointers())
  ;

  class_<DCSimpleParameter, base<DCParameter>>("DCSimpleParameter")
  .property("type", &DCSimpleParameter::get_type)
  .property("hasModulus", &DCSimpleParameter::has_modulus)
  .property("modulus", &DCSimpleParameter::get_modulus)
  .property("divisor", &DCSimpleParameter::get_divisor)
  ;

  class_<DCClassParameter, base<DCParameter>>("DCClassParameter")
  .function("getClass", &DCClassParameter::get_class, allow_raw_pointers())
  ;

  class_<DCArrayParameter, base<DCParameter>>("DCArrayParameter")
  .function("getType", &DCArrayParameter::get_element_type, allow_raw_pointers())
  .property("arraySize", &DCArrayParameter::get_array_size)
  ;

  class_<DCSwitchParameter, base<DCParameter>>("DCSwitchParameter")
  .function("getSwitch", &DCSwitchParameter::get_switch, allow_raw_pointers())
  ;

  class_<DCAtomicField, base<DCField>>("DCAtomicField")
  .property("numElements", &DCAtomicField::get_num_elements)
  .function("getElement", &DCAtomicField::get_element, allow_raw_pointers())
  ;

  class_<DCMolecularField, base<DCField>>("DCMolecularField")
  .property("numAtomics", &DCMolecularField::get_num_atomics)
  .function("getAtomic", &DCMolecularField::get_atomic, allow_raw_pointers())
  ;
}
