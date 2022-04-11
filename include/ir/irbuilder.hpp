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

/// @brief A class for creating and managing instructions
/// 
/// Instructions will be inserted in a @link BasicBlock @endlink. \n
/// Note that this API does not fully expose the uses of instructions.
class IRBuilder
{

public:

	/// Constructs a new IR Builder
	IRBuilder();

};

}

#endif // EVIIR_IR_IRBUILDER_H