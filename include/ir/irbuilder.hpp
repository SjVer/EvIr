//===---------- ir/irbuilder.hpp --- C++ --------===
// This header is part of the EviIr library
// MIT License - Sjoerd Vermeulen (2022)
// For more info see https://github.com/SjVer/EviIr
//===--------------------------------------------===

#ifndef EVIIR_IR_IRBUILDER_H
#define EVIIR_IR_IRBUILDER_H

#include ".common.hpp"
#include "ir/module.hpp"

namespace eviir
{

class IRBuilder
{
	Module* module;

public:
	IRBuilder(Module* module = nullptr);

	Module* get_module();
};

}

#endif // EVIIR_IR_IRBUILDER_H