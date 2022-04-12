//===--- ir/irbuilder.hpp ---------------- C++ ---===
// This header is part of the EvIr library
// MIT License - Sjoerd Vermeulen (2022)
// For more info see https://github.com/SjVer/EvIr
//===---------------------------------------------===

#ifndef EVIR_IR_IRBUILDER_H
#define EVIR_IR_IRBUILDER_H

#include ".common.hpp"
#include "ir/module.hpp"

namespace evir
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

#endif // EVIR_IR_IRBUILDER_H