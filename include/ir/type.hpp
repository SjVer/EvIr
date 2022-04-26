//===--- ir/type.hpp -------------------- C++ ---===
// This header is part of the EvIr library
// MIT License - Sjoerd Vermeulen (2022)
// For more info see https://github.com/SjVer/EvIr
//===--------------------------------------------===

#ifndef EVIR_IR_TYPE_H
#define EVIR_IR_TYPE_H

#include "evir/.common.hpp"

namespace evir {

class IntegerType;
class FloatType;
class PointerType;
class ArrayType;
class FunctionType;

/// The interface for types
class Type
{
protected:
	/// @cond

	static const enum TypeID {
		TYPE_INTEGER,
		TYPE_FLOAT,

		TYPE_POINTER,
		TYPE_ARRAY,

		TYPE_FUNCTION,

		TYPE_none
	} type_id = TYPE_none;

	/// @endcond
public:

	#pragma region getters
	#define GETTER(classname, lowercasename, ...) static classname* get_##lowercasename##_type(__VA_ARGS__)

	/// Constructs a new @link IntegerType @endlink
	GETTER(IntegerType, integer, bool is_signed, uint64 bits_count);

	/// Constructs a new @link FloatType @endlink
	GETTER(FloatType, float, uint64 bits_count);

	/// Constructs a new @link PointerType @endlink
	GETTER(PointerType, pointer, Type* subtype);

	/// Constructs a new @link FunctionType @endlink
	GETTER(FunctionType, function, Type* return_type, Vector<Type*> params);

	#undef GETTER
	#pragma endregion

	#pragma region type id checks
	#define TYPE_CHECK_MEMBER(lowercasename, type) \
		bool is_##lowercasename##_type() { return type_id == type; }

	TYPE_CHECK_MEMBER(integer, TYPE_INTEGER);
	TYPE_CHECK_MEMBER(float, TYPE_FLOAT);
	TYPE_CHECK_MEMBER(pointer, TYPE_POINTER);
	TYPE_CHECK_MEMBER(function, TYPE_FUNCTION);

	#undef TYPE_CHECK_MEMBER
	#pragma endregion

	/// Constructs a pointer to the the current instance's type
	PointerType* get_pointer_to();

	/// Generates the IR for the type
	/// @return the IR as a string (without a newline)
	virtual String generate_ir() = 0;
};

#pragma region subclasses

/// An abritrary-width integer type
class IntegerType : public Type
{
	static const TypeID type_id = TYPE_INTEGER;
	uint64 bitwidth;
	bool is_signed;

	// IntegerType();

public:

	enum {
		MIN_BITS_COUNT = 1,
		MAX_BITS_COUNT = (1 << 23)
	};

	IntegerType(bool is_signed, uint64 bits_count);
	// static IntegerType* get(uint64 bits_count, bool is_signed);

	/// @copydoc Type::generate_ir()
	String generate_ir();
};

/// An abritrary-width float type
class FloatType : public Type
{
	static const TypeID type_id = TYPE_FLOAT;
	uint64 bitwidth;

	// FloatType();

public:

	/// Accepted bitwidths
	enum {
		HALF = 16,
		FLOAT = 32,
		DOUBLE = 64
	};

	FloatType(uint64 bits_count);
	// static FloatType* get(uint64 bits_count);

	/// @copydoc Type::generate_ir()
	String generate_ir();
};

/// A pointer type
class PointerType : public Type
{
	static const TypeID type_id = TYPE_POINTER;
	Type* subtype;

	// FloatType();

public:

	PointerType(Type* subtype);
	// static FloatType* get(uint64 bits_count);

	/// @copydoc Type::generate_ir()
	String generate_ir();
};

/// A function's type
class FunctionType : public Type
{
	static const TypeID type_id = TYPE_FUNCTION;
	Type* return_type;
	Vector<Type*> params;

	// FunctionType();

public:

	FunctionType(Type* return_type, Vector<Type*> params);
	// static FloatType* get(uint64 bits_count);

	/// @copydoc Type::generate_ir()
	String generate_ir();
};

#pragma endregion

}

#endif // EVIR_IR_TYPE_H