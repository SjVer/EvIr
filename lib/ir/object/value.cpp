#include "ir/object/value.hpp"

using namespace evir;

#pragma region Constructors

#define SIMPLE_VALUE_CONTRUCTOR(classname, lowercasename, membertype, membername) \
	classname* Value::new_##lowercasename(membertype membername) \
	{ return new classname(membername); }

SIMPLE_VALUE_CONTRUCTOR(IntegerValue, integer, int64, value);
SIMPLE_VALUE_CONTRUCTOR(FloatValue, float, float2, value);
SIMPLE_VALUE_CONTRUCTOR(StringValue, string, String, value);
SIMPLE_VALUE_CONTRUCTOR(ListValue, list, Vector<Value*>, elements);
SIMPLE_VALUE_CONTRUCTOR(MapValue, map, Map<Value* COMMA Value*>, pairs);
SIMPLE_VALUE_CONTRUCTOR(ReferenceValue, reference, String, name);
SIMPLE_VALUE_CONTRUCTOR(OptionValue, option, String, name);

#undef SIMPLE_VALUE_CONTRUCTOR

#pragma endregion
#pragma region IR Generation
#define GENERATE_IR(classname) String classname::generate_ir(const char* format)

GENERATE_IR(IntegerValue)
{
	if(format) return tools::fstr(format, value);
	else return std::to_string(value);
}

GENERATE_IR(FloatValue)
{
	if(format) return tools::fstr(format, value);
	else return tools::rtrimc(std::to_string(value), '0');
}

GENERATE_IR(StringValue)
{
	if(format) return tools::fstr(format, value.c_str());
	else return '"' + value + '"';
}

GENERATE_IR(ListValue)
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

GENERATE_IR(MapValue)
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

GENERATE_IR(ReferenceValue)
{
	/* if(format) return tools::fstr(format, value);
	else */ return '%' + name;
}

GENERATE_IR(OptionValue)
{
	/* if(format) return tools::fstr(format, value);
	else */ return ':' + name;
}