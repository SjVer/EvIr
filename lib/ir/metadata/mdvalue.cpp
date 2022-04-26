#include "evir/ir/metadata/metadata.hpp"

using namespace evir;

#define GENERATE_IR(classname) String classname::generate_ir()

GENERATE_IR(MDHexValue)
{
	// if(format) return tools::fstr(format, value);
	return tools::fstr("0x%llx", value);
}

GENERATE_IR(MDStringValue)
{
	// if(format) return tools::fstr(format, string.c_str());
	return '"' + string + '"';
}

GENERATE_IR(MDArrayValue)
{
	SStream stream = SStream();
	stream << "[";
	for(auto element : elements)
	{
		stream << element->generate_ir();
		if(element != elements.back()) stream << ", ";
	}
	stream << "]";
	return stream.str();
}

GENERATE_IR(MDMapValue)
{
	if(pairs.size() == 1)
	{
		MDValue* k = pairs.begin()->first;
		MDValue* v = pairs.begin()->second;
		return "{ " + k->generate_ir() + ": " + v->generate_ir() + " }";
	}

	SStream stream = SStream();
	stream << "{" << endl;
	for(auto it = pairs.begin(); it != pairs.end(); it++)
	{
		stream << tab << it->first->generate_ir() << ": ";
		stream << it->second->generate_ir();
		if(it != --pairs.end()) stream << "," << endl;
	}
	stream << endl << "}";
	return stream.str();
}

GENERATE_IR(MDOptionValue)
{
	//
	return ':' + name;
}

GENERATE_IR(MDTypeValue)
{
	//
	return type->generate_ir();
}

GENERATE_IR(MDIRValue)
{
	//
	return value->generate_ir();
}