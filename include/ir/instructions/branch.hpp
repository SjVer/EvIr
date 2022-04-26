//===--- ir/instructions/branch.hpp ----- C++ ---===
// This header is part of the EvIr library
// MIT License - Sjoerd Vermeulen (2022)
// For more info see https://github.com/SjVer/EvIr
//===--------------------------------------------===

#ifndef EVIR_IR_INSTRUCTIONS_BRANCH_H
#define EVIR_IR_INSTRUCTIONS_BRANCH_H

#include "evir/.common.hpp"
#include "evir/ir/instructions/instruction.hpp"
#include "evir/ir/value/value.hpp"

namespace evir {

class RetInst;

class BranchInst : public Instruction
{
protected:
	/// @cond

	static const char ir_prefix = '~';

	/// @endcond
public:

	/// @copydoc Instruction::generate_ir()
	virtual String generate_ir() = 0;
};

#pragma region BranchInst subclasses
#define MEMBERS(name, type, term) \
	private: static constexpr const char* ir_name = name; \
	public: bool is_terminator() const { return term; } \
	String generate_ir();

class RetInst : public BranchInst
{
	Value* retval;
	MEMBERS("ret", BRANCH_INST_RET, true);
	
	RetInst(Value* retval = nullptr): retval(retval) {}
};

#undef MEMBERS
#pragma endregion

}

#endif // EVIR_IR_INSTRUCTIONS_BRANCH_H