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

class StringValue;

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

		VALUE_NONE
	} value_type = VALUE_NONE;

public:

	/// @section Constructors

	/// Constructs a new string value
	/// @param value the string
	static StringValue* new_string(string value);

	/// @section Virtual members

	/// Generates the IR for the value
	/// @return the IR as a string (without newline)
	virtual string generate_ir() = 0;
};

class StringValue : public Value
{
	static const ValueType value_type = VALUE_STRING;
	string value;

public:

	StringValue(string value): value(value) {}
	string generate_ir();
};

};

#endif // EVIIR_IR_VALUE_H