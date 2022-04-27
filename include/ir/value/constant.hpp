//===--- ir/value/constant.hpp ---------- C++ ---===
// This header is part of the EvIr library
// MIT License - Sjoerd Vermeulen (2022)
// For more info see https://github.com/SjVer/EvIr
//===--------------------------------------------===

#ifndef EVIR_IR_VALUE_CONSTANT_H
#define EVIR_IR_VALUE_CONSTANT_H
#define __EVIR_HEADER

#include "evir/.common.hpp"
#include "evir/ir/value/value.hpp"

namespace evir
{

class IntegerConst;
class FloatConst;
class ArrayConst;
class MapConst;
class StringConst;

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

		CONSTANT_none
	} constant_type = CONSTANT_none;

	/// @endcond
public:

	#pragma region Constructors
	#define SIMPLE_CONSTANT_CONTRUCTOR(classname, lowercasename, membertype, membername) \
		static classname* new_##lowercasename(membertype membername);

	SIMPLE_CONSTANT_CONTRUCTOR(IntegerConst, integer, int64, value);
	SIMPLE_CONSTANT_CONTRUCTOR(FloatConst, float, float2, value);
	SIMPLE_CONSTANT_CONTRUCTOR(ArrayConst, array, Vector<Value*>, elements);
	SIMPLE_CONSTANT_CONTRUCTOR(MapConst, map, Map<Value* COMMA Value*>, pairs);
	static StringConst* new_string(String string);

	#undef SIMPLE_CONSTANT_CONTRUCTOR
	#pragma endregion

	#pragma region Constant type checkers
	#define IS_TYPE_METHOD(typename, type) \
		bool is_##typename() { return constant_type == CONSTANT_##type; }

	/// returns `true` if this is an instance of @link IntegerConst @endlink
	IS_TYPE_METHOD(integer, INTEGER);

	/// returns `true` if this is an instance of @link FloatConst @endlink
	IS_TYPE_METHOD(float, FLOAT);

	/// returns `true` if this is an instance of @link ArrayConst @endlink
	IS_TYPE_METHOD(array, ARRAY);

	/// returns `true` if this is an instance of @link MapConst @endlink
	IS_TYPE_METHOD(map, MAP);

	#undef IS_TYPE_METHOD
	#pragma endregion

	/// @copydoc Value::generate_ir()
	virtual String generate_ir() = 0;
};

#pragma region Constant subclasses
#define MEMBERS(type) \
	private: static const ConstantType constant_type = type; \
	public: String generate_ir();
#define SIMPLE_CONSTANT(name, valuetype, membertype, membername) \
	class name : public Constant { MEMBERS(valuetype); \
	private: membertype membername; \
	public: name(membertype membername): membername(membername) {}; }

SIMPLE_CONSTANT(IntegerConst, 	CONSTANT_INTEGER, 	int64, value);
SIMPLE_CONSTANT(FloatConst, 	CONSTANT_FLOAT, 	float2, value);

SIMPLE_CONSTANT(ArrayConst, 	CONSTANT_ARRAY, 	Vector<Value*>, elements);
SIMPLE_CONSTANT(MapConst, 		CONSTANT_MAP, 		Map<Value* COMMA Value*>, pairs);

class StringConst : public ArrayConst
{
	String string;

public:
	StringConst(String string);

	/// @copydoc Constant::generate_ir()
	String generate_ir();
};

#undef MEMBERS
#undef SIMPLE_CONSTANT
#pragma endregion

}

#endif // EVIR_IR_VALUE_CONSTANT_H