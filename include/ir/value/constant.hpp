//===--- ir/object/value.hpp --- C++ ------------===
// This header is part of the EvIr library
// MIT License - Sjoerd Vermeulen (2022)
// For more info see https://github.com/SjVer/EvIr
//===--------------------------------------------===

#ifndef EVIR_IR_OBJECT_CONSTANT_H
#define EVIR_IR_OBJECT_CONSTANT_H

#include ".common.hpp"
#include "ir/value/value.hpp"

namespace evir
{

class IntegerConst;
class FloatConst;
class ArrayConst;
class MapConst;
class ReferenceConst;

class Constant : public Value
{
protected:
	/// @cond

	static const ValueType value_type = VALUE_CONSTANT;
	static const enum ConstantType
	{
		CONSTANT_INTEGER,
		CONSTANT_FLOAT,

		CONSTANT_ARRAY,
		CONSTANT_MAP,

		CONSTANT_REFERENCE,

		CONSTANT_none
	} constant_type = CONSTANT_none;

	/// @endcond
public:

	#pragma region Constructors
	#define SIMPLE_CONSTANT_CONTRUCTOR(classname, lowercasename, membertype, membername) \
		static classname* new_##lowercasename(membertype membername);

	/// Constructs a new integer constant
	/// @param value the integer
	SIMPLE_CONSTANT_CONTRUCTOR(IntegerConst, integer, int64, value);

	/// Constructs a new float constant
	/// @param value the float
	SIMPLE_CONSTANT_CONTRUCTOR(FloatConst, float, float2, value);

	/// Constructs a new array constant
	/// @param elements the elements of the array
	SIMPLE_CONSTANT_CONTRUCTOR(ArrayConst, array, Vector<Value*>, elements);

	/// Constructs a new map constant
	/// @param pair the key-value pairs of the map
	SIMPLE_CONSTANT_CONTRUCTOR(MapConst, map, Map<Value* COMMA Value*>, pairs);

	// /// Constructs a new reference constant
	// /// @param name the name of the reference
	// SIMPLE_CONSTANT_CONTRUCTOR(ReferenceConst, reference, Reference*, user = nullptr);

	#undef SIMPLE_CONSTANT_CONTRUCTOR
	#pragma endregion

	#pragma region Constant type checkers
	#define IS_TYPE_METHOD(typename, type) \
		bool is_##typename() { return constant_type == CONSTANT_##type; }

	IS_TYPE_METHOD(integer, INTEGER);
	IS_TYPE_METHOD(float, FLOAT);
	IS_TYPE_METHOD(array, ARRAY);
	IS_TYPE_METHOD(map, MAP);
	IS_TYPE_METHOD(reference, REFERENCE);

	#undef IS_TYPE_METHOD
	#pragma endregion

	/// Generates the IR for the value
	/// @return the IR as a string (without a newline)
	virtual String generate_ir(const char* format = nullptr) = 0;
};

#pragma region Constant subclasses
#define MEMBERS(type) \
	private: static const ConstantType constant_type = CONSTANT_##type; \
	public: String generate_ir(const char* format = nullptr);
#define SIMPLE_CONSTANT(name, valuetype, membertype, membername) \
	class name : public Constant { MEMBERS(valuetype); \
	name(membertype membername): membername(membername) {}; membertype membername; }

SIMPLE_CONSTANT(IntegerConst, INTEGER, int64, value);
SIMPLE_CONSTANT(FloatConst, FLOAT, float2, value);

SIMPLE_CONSTANT(ArrayConst, ARRAY, Vector<Value*>, elements);
SIMPLE_CONSTANT(MapConst, MAP, Map<Value* COMMA Value*>, pairs);

// SIMPLE_VALUE(ReferenceConst, REFERENCE, Reference*, user);

#undef MEMBERS
#undef SIMPLE_CONSTANT
#pragma endregion

}

#endif // EVIR_IR_OBJECT_CONSTANT_H