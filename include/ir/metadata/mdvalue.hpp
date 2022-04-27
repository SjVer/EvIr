//===--- ir/mdvalue.hpp ----------------- C++ ---===
// This header is part of the EvIr library
// MIT License - Sjoerd Vermeulen (2022)
// For more info see https://github.com/SjVer/EvIr
//===--------------------------------------------===

#ifndef EVIR_IR_MDVALUE_H
#define EVIR_IR_MDVALUE_H
#define __EVIR_HEADER

#include "evir/.common.hpp"
#include "evir/ir/value/value.hpp"
#include "evir/ir/type.hpp"

namespace evir {

/// A Class representing a value used by metadata
class MDValue
{
	friend class Module;

protected:
	/// @cond

	static const enum MDValueType
	{
		MDVALUE_HEX,
		MDVALUE_STRING,
		MDVALUE_ARRAY,
		MDVALUE_MAP,
		MDVALUE_OPTION,
		MDVALUE_TYPE,
		MDVALUE_IRVALUE,

		MDVALUE_none
	} mdvalue_type = MDVALUE_none;

	/// @endcond
public:

	#pragma region MDValue type checkers
	#define IS_TYPE_METHOD(ln, type) \
		bool is_##ln () { return mdvalue_type == MDVALUE_##type; }

	IS_TYPE_METHOD(hex, HEX);
	IS_TYPE_METHOD(string, STRING);
	IS_TYPE_METHOD(array, ARRAY);
	IS_TYPE_METHOD(map, MAP);
	IS_TYPE_METHOD(option, OPTION);
	IS_TYPE_METHOD(type, TYPE);
	IS_TYPE_METHOD(irvalue, IRVALUE);

	#undef IS_TYPE_METHOD
	#pragma endregion

	/// Generates the IR for the value
	/// @return the IR as a string (without a newline)
	virtual String generate_ir() = 0;
};

#define MDVALUE_MEMBERS(type) \
	friend class Module; \
	static const MDValueType mdvalue_type = MDVALUE_##type;

/// @copydoc MDValue
class MDHexValue : public MDValue
{
	unsigned long long value;
	MDVALUE_MEMBERS(HEX);

public:

	MDHexValue(unsigned long long value): value(value) {}

	/// @copydoc MDValue::generate_ir()
	String generate_ir();
};

/// @copydoc MDValue
class MDStringValue : public MDValue
{
	String string;
	MDVALUE_MEMBERS(STRING);

public:

	MDStringValue(String string): string(string) {}

	/// @copydoc MDValue::generate_ir()
	String generate_ir();
};

/// @copydoc MDValue
class MDArrayValue : public MDValue
{
	Vector<MDValue*> elements;
	MDVALUE_MEMBERS(ARRAY);

public:

	MDArrayValue(Vector<MDValue*> elements): elements(elements) {}

	/// @copydoc MDValue::generate_ir()
	String generate_ir();
};

/// @copydoc MDValue
class MDMapValue : public MDValue
{
	Map<MDValue*, MDValue*> pairs;
	MDVALUE_MEMBERS(MAP);

public:

	MDMapValue(Map<MDValue*, MDValue*> pairs): pairs(pairs) {}

	/// @copydoc MDValue::generate_ir()
	String generate_ir();
};

/// @copydoc MDValue
class MDOptionValue : public MDValue
{
	String name;
	MDVALUE_MEMBERS(OPTION);

public:

	MDOptionValue(String name): name(name) {}

	/// @copydoc MDValue::generate_ir()
	String generate_ir();
};

/// @copydoc MDValue
class MDTypeValue : public MDValue
{
	Type* type;
	MDVALUE_MEMBERS(TYPE);

public:

	MDTypeValue(Type* type): type(type) {}

	/// @copydoc MDValue::generate_ir()
	String generate_ir();
};

/// @copydoc MDValue
class MDIRValue : public MDValue
{
	Value* value;
	MDVALUE_MEMBERS(IRVALUE);

public:

	MDIRValue(Value* value): value(value) {}

	/// @copydoc MDValue::generate_ir()
	String generate_ir();
};

#undef MDVALUE_MEMBERS

}

#endif // EVIR_IR_MDVALUE_H