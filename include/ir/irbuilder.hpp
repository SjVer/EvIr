//===--- ir/irbuilder.hpp ---------------- C++ ---===
// This header is part of the EvIr library
// MIT License - Sjoerd Vermeulen (2022)
// For more info see https://github.com/SjVer/EvIr
//===---------------------------------------------===

#ifndef EVIR_IR_IRBUILDER_H
#define EVIR_IR_IRBUILDER_H
#define __EVIR_HEADER

#include "evir/.common.hpp"
#include "evir/.statics_pch.hpp"
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

	/// Inserts the given comment at the end of the current
	/// @link BasicBlock @endlink.
	void insert_comment(String text);

	#pragma region Simple instruction creation
	#define CREATE_INST(cn, ln, s, ...) \
		cn* create_##ln s { return (cn*)insert(new cn(__VA_ARGS__)); }

	CREATE_INST(RetInst, ret, (Value* retval), retval);
	CREATE_INST(BrInst, br, (BasicBlock* dest), dest);
	CREATE_INST(CondBrInst, condbr, (Value* c, BasicBlock* td, BasicBlock* fd), c, td, fd);
	CREATE_INST(CondBrInst, condbr, (Value* c, BasicBlock* td), c, td, nullptr);

	CREATE_INST(DispInst, disp, (Value* expression), expression);

	#undef CREATE_INST
	#pragma endregion

	#pragma region Simple expression creation
	#define CREATE_EXPR(ln, cn, s, ...) \
		Value* create_##ln s { return new cn(__VA_ARGS__); }
	#define CREATE_BIN_EXPR(ln, cn, en) \
		CREATE_EXPR(ln, cn, (Value* lhs, Value* rhs), cn::en, lhs, rhs)

	CREATE_BIN_EXPR(add, BinaryOp, ADD);
	CREATE_BIN_EXPR(sub, BinaryOp, SUB);
	CREATE_BIN_EXPR(mul, BinaryOp, MUL);
	CREATE_BIN_EXPR(div, BinaryOp, DIV);
	CREATE_BIN_EXPR(mod, BinaryOp, MOD);
	CREATE_BIN_EXPR(shr, BinaryOp, SHR);
	CREATE_BIN_EXPR(shl, BinaryOp, SHL);
	CREATE_BIN_EXPR(and, BinaryOp, AND);
	CREATE_BIN_EXPR(or,  BinaryOp, OR);
	CREATE_BIN_EXPR(xor, BinaryOp, XOR);

	CREATE_BIN_EXPR(cmpeq,   ComparisonOp, EQ);
	CREATE_BIN_EXPR(cmpneq,  ComparisonOp, NEQ);
	CREATE_BIN_EXPR(cmplt,   ComparisonOp, LT);
	CREATE_BIN_EXPR(cmplteq, ComparisonOp, LTEQ);
	CREATE_BIN_EXPR(cmpgt,   ComparisonOp, GT);
	CREATE_BIN_EXPR(cmpgteq, ComparisonOp, GTEQ);

	CREATE_EXPR(call, CallOp, (Function* callee, Vector<Value*> args), callee, args);

	#undef CREATE_BIN_EXPR
	#undef CREATE_EXPR
	#pragma endregion

	#pragma region Advanced creation

	/// Creates a return instruction that returns the
	/// "nullified" variant of the function's return type
	// RetInst* create_ret_null();

	#pragma endregion

	#pragma endregion
	#pragma region Types
	#define GET_TYPE_METHOD(cn, ln, ...) \
		cn* get_##ln##_type() { return (cn*)&static_##ln##_type; }

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
	IntegerType* get_uintn_type(uint bitwidth) {
		return new IntegerType(false, bitwidth); }
	IntegerType* get_uint_type(uint bitwidth) {
		return new IntegerType(true, bitwidth); }
	GET_TYPE_METHOD(FloatType, half, 16);
	GET_TYPE_METHOD(FloatType, float, 32);
	GET_TYPE_METHOD(FloatType, double, 64);
	GET_TYPE_METHOD(VoidType, void);

	#undef GET_TYPE_METHOD
	#pragma endregion
};

}

#endif // EVIR_IR_IRBUILDER_H