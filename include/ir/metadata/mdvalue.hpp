//===--- ir/mdvalue.hpp ----------------- C++ ---===
// This header is part of the EvIr library
// MIT License - Sjoerd Vermeulen (2022)
// For more info see https://github.com/SjVer/EvIr
//===--------------------------------------------===

#ifndef EVIR_IR_MDVALUE_H
#define EVIR_IR_MDVALUE_H

#include "evir/.common.hpp"
#include "evir/ir/value/value.hpp"
#include "evir/ir/type.hpp"

namespace evir {

/// A Class representing a value used by metadata
class MDValue
{
public:
	/// Generates the IR for the value
	/// @return the IR as a string (without a newline)
	virtual String generate_ir() = 0;
};



/// @copydoc MDValue
class MDHexValue : public MDValue
{
	unsigned long long value;

public:

	MDHexValue(unsigned long long value): value(value) {}

	/// @copydoc MDValue::generate_ir()
	String generate_ir();
};

/// @copydoc MDValue
class MDStringValue : public MDValue
{
	String string;

public:

	MDStringValue(String string): string(string) {}

	/// @copydoc MDValue::generate_ir()
	String generate_ir();
};

/// @copydoc MDValue
class MDArrayValue : public MDValue
{
	Vector<MDValue*> elements;

public:

	MDArrayValue(Vector<MDValue*> elements): elements(elements) {}

	/// @copydoc MDValue::generate_ir()
	String generate_ir();
};

/// @copydoc MDValue
class MDMapValue : public MDValue
{
	Map<MDValue*, MDValue*> pairs;

public:

	MDMapValue(Map<MDValue*, MDValue*> pairs): pairs(pairs) {}

	/// @copydoc MDValue::generate_ir()
	String generate_ir();
};

/// @copydoc MDValue
class MDOptionValue : public MDValue
{
	String name;

public:

	MDOptionValue(String name): name(name) {}

	/// @copydoc MDValue::generate_ir()
	String generate_ir();
};

/// @copydoc MDValue
class MDTypeValue : public MDValue
{
	Type* type;

public:

	MDTypeValue(Type* type): type(type) {}

	/// @copydoc MDValue::generate_ir()
	String generate_ir();
};

/// @copydoc MDValue
class MDIRValue : public MDValue
{
	Value* value;

public:

	MDIRValue(Value* value): value(value) {}

	/// @copydoc MDValue::generate_ir()
	String generate_ir();
};

}

#endif // EVIR_IR_MDVALUE_H