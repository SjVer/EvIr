//===--- ir/irbuilder.hpp ---------------- C++ ---===
// This header is part of the EvIr library
// MIT License - Sjoerd Vermeulen (2022)
// For more info see https://github.com/SjVer/EvIr
//===---------------------------------------------===

#ifndef EVIR_IR_IRBUILDER_H
#define EVIR_IR_IRBUILDER_H
#define __EVIR_HEADER

#include "evir/.common.hpp"
#include "evir/ir/module.hpp"
#include "evir/ir/basicblock.hpp"
#include "evir/ir/instructions/all.hpp"
#include "evir/ir/value/all.hpp"

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

	#pragma region Instruction creation
	#define CREATE_INST(cn, ln, s, ...) \
		cn* create_##ln s { return (cn*)insert(new cn(__VA_ARGS__)); }

	CREATE_INST(RetInst, ret, (Value* retval), retval);
	CREATE_INST(BrInst, br, (BasicBlock* dest), dest);
	CREATE_INST(CondBrInst, condbr, (Value* c, BasicBlock* td, BasicBlock* fd), c, td, fd);
	CREATE_INST(CondBrInst, condbr, (Value* c, BasicBlock* td), c, td, nullptr);

	CREATE_INST(DispInst, disp, (Value* expression), expression);

	#undef CREATE_INST
	#pragma endregion

	#pragma endregion
	#pragma region Types
	#define GET_TYPE_METHOD(cn, ln, ...) \
		cn* get_##ln##_type() { return new cn(__VA_ARGS__); }

	GET_TYPE_METHOD(IntegerType, bool, false, 1);
	GET_TYPE_METHOD(IntegerType, uint8, false, 8);
	GET_TYPE_METHOD(IntegerType, int8, true, 8);
	GET_TYPE_METHOD(IntegerType, uint16, false, 16);
	GET_TYPE_METHOD(IntegerType, int16, true, 16);
	GET_TYPE_METHOD(IntegerType, uint32, false, 32);
	GET_TYPE_METHOD(IntegerType, int32, true, 32);
	GET_TYPE_METHOD(IntegerType, uint64, false, 64);
	GET_TYPE_METHOD(IntegerType, int64, true, 64);
	GET_TYPE_METHOD(IntegerType, uint128, false, 128);
	GET_TYPE_METHOD(IntegerType, int128, true, 128);
	GET_TYPE_METHOD(FloatType, half, 16);
	GET_TYPE_METHOD(FloatType, float, 32);
	GET_TYPE_METHOD(FloatType, double, 64);
	GET_TYPE_METHOD(VoidType, void);

	#undef GET_TYPE_METHOD
	#pragma endregion
};

}

#endif // EVIR_IR_IRBUILDER_H