//===---------- ir/value.hpp --- C++ ------------===
// This header is part of the EvIr library
// MIT License - Sjoerd Vermeulen (2022)
// For more info see https://github.com/SjVer/EvIr
//===--------------------------------------------===

#ifndef EVIR_IR_VALUE_H
#define EVIR_IR_VALUE_H

#include ".common.hpp"
#include "ir/object/object.hpp"

namespace evir
{

class IntegerValue;
class FloatValue;
class StringValue;
class ListValue;
class MapValue;
class ReferenceValue;
class OptionValue;

class Value : public Object
{
protected:
	/// @cond

	static const ObjectType object_type = OBJECT_VALUE;

	static const enum ValueType
	{
		VALUE_INTEGER,
		VALUE_FLOAT,
		VALUE_STRING,

		VALUE_LIST,
		VALUE_MAP,

		VALUE_EXPRESSION,
		VALUE_REFERENCE,
		VALUE_OPTION,

		VALUE_none
	} value_type = VALUE_none;

	/// @endcond
public:

	#pragma region Constructors
	#define SIMPLE_VALUE_CONTRUCTOR(classname, lowercasename, membertype, membername) \
		static classname* new_##lowercasename(membertype membername);

	/// Constructs a new integer value
	/// @param value the integer
	SIMPLE_VALUE_CONTRUCTOR(IntegerValue, integer, int64, value);

	/// Constructs a new float value
	/// @param value the float
	SIMPLE_VALUE_CONTRUCTOR(FloatValue, float, float2, value);

	/// Constructs a new string value
	/// @param value the string
	SIMPLE_VALUE_CONTRUCTOR(StringValue, string, String, value);

	/// Constructs a new list value
	/// @param elements the elements of the list
	SIMPLE_VALUE_CONTRUCTOR(ListValue, list, Vector<Value*>, elements);

	/// Constructs a new map value
	/// @param pair the key-value pairs of the map
	SIMPLE_VALUE_CONTRUCTOR(MapValue, map, Map<Value* COMMA Value*>, pairs);

	/// Constructs a new reference value
	/// @param name the name of the reference
	SIMPLE_VALUE_CONTRUCTOR(ReferenceValue, reference, String, name);

	/// Constructs a new option value
	/// @param name the name of the option
	SIMPLE_VALUE_CONTRUCTOR(OptionValue, option, String, name);

	#undef SIMPLE_VALUE_CONTRUCTOR
	#pragma endregion

	#pragma region Value type checkers
	#define IS_TYPE_METHOD(typename, type) \
		bool is_##typename() { return value_type == VALUE_##type; }

	IS_TYPE_METHOD(integer, INTEGER);
	IS_TYPE_METHOD(float, FLOAT);
	IS_TYPE_METHOD(string, STRING);
	IS_TYPE_METHOD(list, LIST);
	IS_TYPE_METHOD(map, MAP);
	IS_TYPE_METHOD(reference, REFERENCE);
	IS_TYPE_METHOD(option, OPTION);

	#undef IS_TYPE_METHOD
	#pragma endregion

	/// Generates the IR for the value
	/// @return the IR as a string (without a newline)
	virtual String generate_ir(const char* format = nullptr) = 0;
};

#pragma region Value classes
#define MEMBERS(type) \
	private: static const ValueType value_type = type; \
	public: String generate_ir(const char* format = nullptr);
#define SIMPLE_VALUE(name, valuetype, membername, membertype) \
	class name : public Value { MEMBERS(valuetype); \
	name(membertype membername): membername(membername) {}; membertype membername; }

SIMPLE_VALUE(IntegerValue, VALUE_INTEGER, value, int64);
SIMPLE_VALUE(FloatValue, VALUE_FLOAT, value, float2);
SIMPLE_VALUE(StringValue, VALUE_STRING, value, String);

SIMPLE_VALUE(ListValue, VALUE_LIST, elements, Vector<Value*>);
SIMPLE_VALUE(MapValue, VALUE_MAP, pairs, Map<Value* COMMA Value*>);

SIMPLE_VALUE(ReferenceValue, VALUE_REFERENCE, name, String);
SIMPLE_VALUE(OptionValue, VALUE_OPTION, name, String);

#undef MEMBERS
#undef SIMPLE_VALUE
#pragma endregion

};

#endif // EVIR_IR_VALUE_H