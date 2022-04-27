//===--- ir/instructions/branch.hpp ----- C++ ---===
// This header is part of the EvIr library
// MIT License - Sjoerd Vermeulen (2022)
// For more info see https://github.com/SjVer/EvIr
//===--------------------------------------------===

#ifndef EVIR_IR_INSTRUCTIONS_BRANCH_H
#define EVIR_IR_INSTRUCTIONS_BRANCH_H
#define __EVIR_HEADER

#include "evir/.common.hpp"
#include "evir/ir/instructions/instruction.hpp"
#include "evir/ir/value/value.hpp"

namespace evir {

class BranchInst : public Instruction
{
protected:
	/// @cond

	static const char ir_prefix = '~';

	virtual void resolve() = 0;

	/// @endcond
public:

	/// @copydoc Instruction::generate_ir()
	virtual String generate_ir() = 0;
};

#pragma region BranchInst subclasses
#define MEMBERS(name, type, term) \
	private: void resolve(); \
	static const InstType inst_type = type;\
	static constexpr const char* ir_name = name; \
	public: bool is_terminator() const { return term; } \
	String generate_ir();

class RetInst : public BranchInst
{
	Value* retval;
	MEMBERS("ret", INST_RET, true);
	
	RetInst(Value* retval): retval(retval) {}
};

class BrInst : public BranchInst
{
	BasicBlock* dest;
	MEMBERS("br", INST_BR, true);
	
	BrInst(BasicBlock* dest): dest(dest) {}
};

class CondBrInst : public BranchInst
{
	Value* condition;
	BasicBlock* true_dest;
	BasicBlock* false_dest;
	MEMBERS("condbr", INST_CONDBR, true_dest && false_dest);
	
	/// @param false_dest is optional
	CondBrInst(Value* condition,
			   BasicBlock* true_dest, 
			   BasicBlock* false_dest = nullptr):
		condition(condition), true_dest(true_dest),
		false_dest(false_dest) {}
};

#undef MEMBERS
#pragma endregion

}

#endif // EVIR_IR_INSTRUCTIONS_BRANCH_H