#include "ir/type.hpp"

using namespace evir;

#pragma region getters
#define GETTER(classname, lowercasename, ...) classname* Type::get_##lowercasename##_type(__VA_ARGS__)
#define BODY(classname, ...) { return new classname(__VA_ARGS__); }

GETTER(IntegerType, integer, bool is_signed, uint64 bits_count) BODY(IntegerType, is_signed, bits_count);
GETTER(FloatType, float, uint64 bits_count) BODY(FloatType, bits_count);
GETTER(PointerType, pointer, Type* subtype) BODY(PointerType, subtype);
GETTER(FunctionType, function, Type* return_type, Vector<Type*> params) BODY(FunctionType, return_type, params);

#undef BODY
#undef GETTER
#pragma endregion

PointerType* Type::get_pointer_to()
{
	return new PointerType(this);
}


IntegerType::IntegerType(bool is_signed, uint64 bits_count)
// IntegerType* IntegerType::get(uint64 bits_count, bool is_signed)
{
	ASSERT(bits_count >= MIN_BITS_COUNT, "bitwidth too small!");
	ASSERT(bits_count <= MAX_BITS_COUNT, "bitwidth too large!");

	// IntegerType* type = new IntegerType();
	// type->bitwidth = bits_count;
	// type->is_signed = is_signed;
	// return type;

	bitwidth = bits_count;
	this->bitwidth = bits_count;
	this->is_signed = is_signed;
}

String IntegerType::generate_ir()
{
	return (String)"i."
		+ (is_signed ? "s." : "u.")
		+ std::to_string(bitwidth);
}


FloatType::FloatType(uint64 bits_count)
{
	// check validity of bitwidth
	switch(bits_count)
	{
		case HALF: case FLOAT: case DOUBLE: break;
		default: ASSERT(0, "invalid FloatType bitwidth!");
	}

	bitwidth = bits_count;
}

String FloatType::generate_ir()
{
	//
	return "f." + std::to_string(bitwidth);
}


PointerType::PointerType(Type* subtype_)
{
	subtype = subtype_;
}

String PointerType::generate_ir()
{
	ASSERT(subtype, "pointer subtype is null!");
	return subtype->generate_ir() + '*';
}


FunctionType::FunctionType(Type* return_type_, Vector<Type*> params_)
{
	return_type = return_type_;
	params = params_;
}

String FunctionType::generate_ir()
{
	SStream stream = SStream();

	ASSERT(return_type, "function return type is null!");
	stream << return_type->generate_ir() + " (";

	for(auto param : params)
	{
		ASSERT(param, "function parameter type is null!");
		stream << param->generate_ir();
		if(param != params.back()) stream << " ";
	}
	stream << ")";

	return stream.str();
}

