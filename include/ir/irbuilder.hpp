//===--- ir/irbuilder.hpp ---------------- C++ ---===
// This header is part of the EvIr library
// MIT License - Sjoerd Vermeulen (2022)
// For more info see https://github.com/SjVer/EvIr
//===---------------------------------------------===

#ifndef EVIR_IR_IRBUILDER_H
#define EVIR_IR_IRBUILDER_H

#include ".common.hpp"
#include "ir/module.hpp"
#include "ir/basicblock.hpp"

namespace evir
{

/// @brief A class for creating and managing instructions
/// 
/// Instructions will be inserted in a @link BasicBlock @endlink. \n
/// Note that this API does not fully expose the uses of instructions.
class IRBuilder
{

	BasicBlock* bblock = nullptr;

public:

	/// Constructs a new IR Builder
	/// @param bblock a block to start appending instructions to
	IRBuilder(BasicBlock* bblock = nullptr);

	#pragma region Management

	BasicBlock* get_block() { return bblock; }
	void set_block(BasicBlock* new_block) { bblock = new_block; }

	#pragma endregion
	#pragma region IR Manipulation

	/// Inserts the given instruction at the end of the current
	/// @link BasicBlock @endlink and returns it.
	Instruction* insert(Instruction* inst);

	#pragma endregion
};

}

#endif // EVIR_IR_IRBUILDER_H