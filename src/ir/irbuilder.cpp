#include "ir/irbuilder.hpp"

using namespace eviir;

IRBuilder::IRBuilder(Module* module): module(module)
{

}

Module* IRBuilder::get_module()
{
	return module;
}