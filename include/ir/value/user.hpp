//===---- ir/object/user.hpp --- C++ ------------===
// This header is part of the EvIr library
// MIT License - Sjoerd Vermeulen (2022)
// For more info see https://github.com/SjVer/EvIr
//===--------------------------------------------===

#ifndef EVIR_IR_OBJECT_USER_H
#define EVIR_IR_OBJECT_USER_H

#include ".common.hpp"
#include "ir/object/object.hpp"

namespace evir
{

class User : public Object
{
protected:
	/// @cond

	static const ObjectType object_type = OBJECT_USER;
	static const enum UserType
	{
		USER_VARIABLE,
		USER_FUNCTION,
		USER_OPERATOR,

		USER_none
	} user_type = USER_none;

	/// @endcond
public:

	#pragma region Constructors

	#pragma endregion
	#pragma region Object type checks
	#define IS_TYPE_METHOD(typename, type) \
		bool is_##typename() { return user_type == USER_##type; }

	/// returns `true` if this is an instance of @link VariableUser @endlink
	IS_TYPE_METHOD(variable, VARIABLE);

	/// returns `true` if this is an instance of @link FunctionUser @endlink
	IS_TYPE_METHOD(function, FUNCTION);

	/// returns `true` if this is an instance of @link OperatorUser @endlink
	IS_TYPE_METHOD(operator, OPERATOR);

	#undef IS_TYPE_METHOD
	#pragma endregion

	/// Generates the IR for the object
	/// @return the IR as a string (without a newline)
	virtual String generate_ir(const char* format = nullptr) = 0;
};

};

#endif // EVIR_IR_OBJECT_USER_H