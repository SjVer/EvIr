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

/// A class for building an managing modules
class IRBuilder
{
	Module* module;

public:

	/// Constructs a new IR Builder
	/// @param module the builder's current module (optional)
	IRBuilder(Module* module = nullptr);

	/// @return a pointer to the builder's current module
	Module* get_module();
};

}

#endif // EVIIR_IR_IRBUILDER_H