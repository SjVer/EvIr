//===--- ir/object/object.hpp --- C++ -----------===
// This header is part of the EvIr library
// MIT License - Sjoerd Vermeulen (2022)
// For more info see https://github.com/SjVer/EvIr
//===--------------------------------------------===

#ifndef EVIR_IR_OBJECT_OBJECT_H
#define EVIR_IR_OBJECT_OBJECT_H

namespace evir {

class Value;
class User;
class Type;

class Object
{
protected:
	/// @cond

	static const enum ObjectType
	{
		OBJECT_VALUE,
		OBJECT_USER,
		OBJECT_TYPE,

		OBJECT_none,
	} object_type = OBJECT_none;

	/// @endcond
public:

	#pragma region Object type checks
	#define IS_TYPE_METHOD(typename, type) \
		bool is_##typename() { return object_type == OBJECT_##type; }

	/// returns `true` if this is an instance of @link Value @endlink.
	IS_TYPE_METHOD(value, VALUE);

	/// returns `true` if this is an instance of @link User @endlink.
	IS_TYPE_METHOD(user, USER);

	/// returns `true` if this is an instance of @link Type @endlink.
	IS_TYPE_METHOD(type, TYPE);

	#undef IS_TYPE_METHOD
	#pragma endregion

	/// Generates the IR for the object
	/// @return the IR as a string (without a newline)
	virtual String generate_ir(const char* format = nullptr) = 0;
};

}

#endif // EVIR_IR_OBJECT_OBJECT_H