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

#undef SIMPLE_CONSTANT_CONTRUCTOR

#pragma endregion
#pragma region IR Generation

String IntegerConst::generate_ir()
{
	// if(format) return tools::fstr(format, value);
	// else
	return std::to_string(value);
}

String FloatConst::generate_ir()
{
	// if(format) return tools::fstr(format, value);
	// else
	String ir = tools::rtrimc(std::to_string(value), '0');
	return ir.back() == '.' ? ir + '0' : ir;
}

String ArrayConst::generate_ir()
{
	SStream stream = SStream();
	stream << "[";
	for(auto element : elements)
	{
		ASSERT(element, "array constant element is null!");
		stream << element->generate_ir();
		if(element != elements.back()) stream << ", ";
	}
	stream << "]";
	return stream.str();
}

String MapConst::generate_ir()
{
	if(pairs.size() == 1)
	{
		Value* k = pairs.begin()->first;
		Value* v = pairs.begin()->second;

		ASSERT(k, "map constant key is null!");
		ASSERT(v, "map constant value is null!");

		return "{ " + k->generate_ir() + ": " + v->generate_ir() + " }";
	}

	SStream stream = SStream();
	stream << "{" << endl;
	for(auto it = pairs.begin(); it != pairs.end(); it++)
	{
		ASSERT(it->first, "map constant key is null!");
		ASSERT(it->second, "map constant value is null!");

		stream << tab << it->first->generate_ir() << ": ";
		stream << it->second->generate_ir();
		if(it != --pairs.end()) stream << "," << endl;
	}
	stream << endl << "}";
	return stream.str();
}

