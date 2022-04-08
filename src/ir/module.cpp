#include "ir/module.hpp"

using namespace eviir;

Module::Module(string name): name(name)
{
	
}

string Module::generate_metadata_ir()
{
	return "!module/name \"" + name + "\"";
}

string Module::generate_ir()
{
	sstream stream = sstream();

	// generate metata ir
	stream << "; =========== module metadata ==========" << std::endl;
	stream << generate_metadata_ir() << std::endl;

	// return string
	return stream.str();
}