//===--- ir/instructions/instruction.hpp --- C++ ---===
// This header is part of the EvIr library
// MIT License - Sjoerd Vermeulen (2022)
// For more info see https://github.com/SjVer/EvIr
//===-----------------------------------------------===

#ifndef EVIR_IR_INSTRUCTIONS_INSTRUCTION_H
#define EVIR_IR_INSTRUCTIONS_INSTRUCTION_H

#include "evir/.common.hpp"

namespace evir {

class BasicBlock;

class BranchInst;
class StorageInst;
class DeclarationInst;

class Instruction
{
protected:
	/// @cond
	
	BasicBlock* parent;

	static const enum InstType
	{
		INST_branch_start,
		INST_RET,
		INST_BR,
		INST_CONDBR,
		INST_branch_end,

		INST_none,
	} inst_type = INST_none;

	static const char ir_prefix;
	static const char* ir_name;
	#define _INST_IR_START (String() + ir_prefix + ir_name + ' ')

	/// @endcond
public:

	#pragma region Instruction type checks
	#define IS_TYPE_METHOD(tn) \
		bool is_##tn##_type_inst() const \
		{ return inst_type > INST_##tn##_start && inst_type < INST_##tn##_end; }
	#define IS_INST_METHOD(instname, type) \
		bool is_##instname##_inst() const { return inst_type == INST_##type; }

	IS_TYPE_METHOD(branch);
	IS_INST_METHOD(ret, RET);
	IS_INST_METHOD(br, BR);
	IS_INST_METHOD(condbr, CONDBR);

	#undef IS_TYPE_METHOD
	#undef IS_INST_METHOD
	#pragma endregion

	BasicBlock* get_parent() const { return parent; }
	// Module* get_module() const {}

	virtual bool is_terminator() const = 0;

	/// Generates the IR for the instruction
	/// @return the IR as a string (with a newline)
	virtual String generate_ir() = 0;
};

}

#endif // EVIR_IR_INSTRUCTIONS_INSTRUCTION_H