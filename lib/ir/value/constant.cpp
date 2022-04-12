#include "ir/value/constant.hpp"

using namespace evir;

#pragma region Constructors

#define SIMPLE_CONSTANT_CONTRUCTOR(classname, lowercasename, membertype, membername) \
	classname* Constant::new_##lowercasename(membertype membername) \
	{ return new classname(membername); }

SIMPLE_CONSTANT_CONTRUCTOR(IntegerConst, integer, int64, value);
SIMPLE_CONSTANT_CONTRUCTOR(FloatConst, float, float2, value);
SIMPLE_CONSTANT_CONTRUCTOR(ArrayConst, array, Vector<Value*>, elements);
SIMPLE_CONSTANT_CONTRUCTOR(MapConst, map, Map<Value* COMMA Value*>, pairs);
// SIMPLE_CONSTANT_CONTRUCTOR(ReferenceConst, reference, Reference*, user);

#undef SIMPLE_CONSTANT_CONTRUCTOR

#pragma endregion
#pragma region IR Generation
#define GENERATE_IR(classname) String classname::generate_ir(const char* format)

GENERATE_IR(IntegerConst)
{
	if(format) return tools::fstr(format, value);
	else return std::to_string(value);
}

GENERATE_IR(FloatConst)
{
	if(format) return tools::fstr(format, value);
	else return tools::rtrimc(std::to_string(value), '0');
}

GENERATE_IR(ArrayConst)
{
	SStream stream = SStream();
	stream << "[";
	for(auto element : elements)
	{
		stream << element->generate_ir(format);
		if(element != elements.back()) stream << ", ";
	}
	stream << "]";
	return stream.str();
}

GENERATE_IR(MapConst)
{
	if(pairs.size() == 1)
	{
		Value* k = pairs.begin()->first;
		Value* v = pairs.begin()->second;
		return "{ " + k->generate_ir(format) + ": " + v->generate_ir(format) + " }";
	}

	SStream stream = SStream();
	stream << "{" << endl;
	for(auto it = pairs.begin(); it != pairs.end(); it++)
	{
		stream << tab << it->first->generate_ir(format) << ": ";
		stream << it->second->generate_ir(format);
		if(it != pairs.end()) stream << "," << endl;
	}
	stream << "}";
	return stream.str();
}

// GENERATE_IR(ReferenceConst)
// {
// 	if(user) return "%"; // '%' + user->get_identifier();
// 	else return "%<unresolved reference>";
// }