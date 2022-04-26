#include "evir/ir/type.hpp"

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

Type* Type::get_pointer_to(uint depth)
{
	if(!depth) return this;
	return new PointerType(get_pointer_to(depth - 1));
}

#pragma region subclasses
#define OPERATOR_EQ_START(cls) \
	if(type_id != rhs.type_id) return false; \
	const cls& r = (const cls&)rhs

#pragma region integer
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

bool IntegerType::operator==(const Type& rhs)
{
	OPERATOR_EQ_START(IntegerType);
	return is_signed == r.is_signed && bitwidth == r.bitwidth;
}

String IntegerType::generate_ir()
{
	return (String)"i."
		+ (is_signed ? "s." : "u.")
		+ std::to_string(bitwidth);
}
#pragma endregion

#pragma region float
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

bool FloatType::operator==(const Type& rhs)
{
	OPERATOR_EQ_START(FloatType);
	return bitwidth == r.bitwidth;
}

String FloatType::generate_ir()
{
	//
	return "f." + std::to_string(bitwidth);
}
#pragma endregion

#pragma region pointer
PointerType::PointerType(Type* subtype_)
{
	subtype = subtype_;
}

bool PointerType::operator==(const Type& rhs)
{
	OPERATOR_EQ_START(PointerType);
	return subtype->operator==(*r.subtype);
}

String PointerType::generate_ir()
{
	ASSERT(subtype, "pointer subtype is null!");
	return subtype->generate_ir() + '*';
}
#pragma endregion

#pragma region function
FunctionType::FunctionType(Type* return_type_, Vector<Type*> params_)
{
	return_type = return_type_;
	params = params_;
}

bool FunctionType::operator==(const Type& rhs)
{
	OPERATOR_EQ_START(FunctionType);
	if(params.size() != r.params.size()) return false;
	else if(return_type->operator!=(*r.return_type)) return false;
	
	for(int i = 0; i < params.size(); i++)
		if(params[i]->operator!=(*r.params[i])) return false;

	return true;
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
#pragma endregion

#pragma endregion