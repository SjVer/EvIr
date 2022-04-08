#include "ir/module.hpp"

using namespace eviir;

Module::Module(string name): name(name)
{
	
}

string Module::to_string()
{
	return ("!module/name \"" + name) + "\"\n";
}