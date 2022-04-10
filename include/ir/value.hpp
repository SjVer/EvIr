//===---------- ir/value.hpp --- C++ -----------===
// This header is part of the EviIr library
// MIT License - Sjoerd Vermeulen (2022)
// For more info see https://github.com/SjVer/EviIr
//===--------------------------------------------===

#ifndef EVIIR_IR_VALUE_H
#define EVIIR_IR_VALUE_H

#include ".common.hpp"

namespace eviir
{

class IntegerValue;
class FloatValue;
class StringValue;
class ListValue;
class ObjectValue;
class ReferenceValue;
class OptionValue;

class Value
{
protected:

	static const enum ValueType
	{
		VALUE_INTEGER,
		VALUE_FLOAT,
		VALUE_STRING,

		VALUE_LIST,
		VALUE_OBJECT,

		VALUE_EXPRESSION,
		VALUE_REFERENCE,
		VALUE_OPTION,

		VALUE_NONE
	} value_type = VALUE_NONE;

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
	SIMPLE_VALUE_CONTRUCTOR(StringValue, string, string, value);

	/// Constructs a new list value
	/// @param elements the elements of the list
	SIMPLE_VALUE_CONTRUCTOR(ListValue, list, vector<Value*>, elements);

	/// Constructs a new object value
	/// @param pair the key-value pairs of the object
	SIMPLE_VALUE_CONTRUCTOR(ObjectValue, object, map<Value* COMMA Value*>, pairs);

	/// Constructs a new reference value
	/// @param name the name of the reference
	SIMPLE_VALUE_CONTRUCTOR(ReferenceValue, reference, string, name);

	/// Constructs a new option value
	/// @param name the name of the option
	SIMPLE_VALUE_CONTRUCTOR(OptionValue, option, string, name);

	#undef SIMPLE_VALUE_CONTRUCTOR
	#pragma endregion

	/// Generates the IR for the value
	/// @return the IR as a string (without newline)
	virtual string generate_ir(const char* format = nullptr) = 0;
};

#pragma region macros
#define MEMBERS(type) \
	private: static const ValueType value_type = type; \
	public: string generate_ir(const char* format = nullptr);
#define SIMPLE_VALUE(name, valuetype, membername, membertype) \
	class name : public Value { MEMBERS(valuetype); \
	name(membertype membername): membername(membername) {}; membertype membername; }
#pragma endregion

SIMPLE_VALUE(IntegerValue, VALUE_INTEGER, value, int64);
SIMPLE_VALUE(FloatValue, VALUE_FLOAT, value, float2);
SIMPLE_VALUE(StringValue, VALUE_STRING, value, string);

SIMPLE_VALUE(ListValue, VALUE_LIST, elements, vector<Value*>);
SIMPLE_VALUE(ObjectValue, VALUE_OBJECT, pairs, map<Value* COMMA Value*>);

SIMPLE_VALUE(ReferenceValue, VALUE_REFERENCE, name, string);
SIMPLE_VALUE(OptionValue, VALUE_OPTION, name, string);

#undef MEMBERS
#undef SIMPLE_VALUE

};

#endif // EVIIR_IR_VALUE_H