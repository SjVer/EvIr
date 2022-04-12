//===--- ir/object/value.hpp ------------ C++ ---===
// This header is part of the EvIr library
// MIT License - Sjoerd Vermeulen (2022)
// For more info see https://github.com/SjVer/EvIr
//===--------------------------------------------===

#ifndef EVIR_IR_OBJECT_VALUE_H
#define EVIR_IR_OBJECT_VALUE_H

#include ".common.hpp"

namespace evir {

class Value
{
protected:
	/// @cond

	static const enum ValueType
	{
		VALUE_CONSTANT,
		VALUE_OPERATOR,
		VALUE_REFERENCE,

		VALUE_none,
	} value_type = VALUE_none;

	/// @endcond
public:

	#pragma region Object type checks
	#define IS_TYPE_METHOD(typename, type) \
		bool is_##typename() { return value_type == VALUE_##type; }

	/// returns `true` if this is an instance of @link Constant @endlink
	IS_TYPE_METHOD(constant, CONSTANT);

	/// returns `true` if this is an instance of @link Operator @endlink
	IS_TYPE_METHOD(operator, OPERATOR);

	/// returns `true` if this is an instance of @link Reference @endlink
	IS_TYPE_METHOD(reference, REFERENCE);

	#undef IS_TYPE_METHOD
	#pragma endregion

	/// Generates the IR for the value
	/// @return the IR as a string (without a newline)
	virtual String generate_ir(const char* format = nullptr) = 0;
};

}

#endif // EVIR_IR_OBJECT_VALUE_H