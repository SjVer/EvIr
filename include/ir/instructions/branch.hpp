//===--- ir/instructions/branch.hpp ----- C++ ---===
// This header is part of the EvIr library
// MIT License - Sjoerd Vermeulen (2022)
// For more info see https://github.com/SjVer/EvIr
//===--------------------------------------------===

#ifndef EVIR_IR_INSTRUCTIONS_BRANCH_H
#define EVIR_IR_INSTRUCTIONS_BRANCH_H

#include ".common.hpp"
#include "ir/instructions/instruction.hpp"
#include "ir/value/value.hpp"

namespace evir {

class RetInst;

class BranchInst : public Instruction
{
protected:
	/// @cond

	static const InstType inst_type = INST_BRANCH;
	static const enum BranchInstType
	{
		BRANCH_INST_RET,
		BRANCH_INST_BR,
		BRANCH_INST_CONDBR,

		BRANCH_INST_none
	} branch_inst_type = BRANCH_INST_none;

	static const char ir_prefix = '~';

	/// @endcond
public:

	#pragma region Constructors
	#define SIMPLE_CONSTRUCTOR(classname, lowercasename, membertype, membername) \
		static classname* new_##lowercasename##_inst(membertype membername);

	/// Constructs a new return instruction
	/// @param retval the return value
	SIMPLE_CONSTRUCTOR(RetInst, ret, Value*, retval = nullptr);

	#undef SIMPLE_CONSTRUCTOR
	#pragma endregion

	#pragma region BranchInst type checkers
	#define IS_TYPE_METHOD(typename, type) \
		bool is_##typename##__inst() { return branch_inst_type == BRANCH_INST_##type; }

	/// returns `true` if this is an instance of @link RetInst @endlink
	IS_TYPE_METHOD(ret, RET);

	/// returns `true` if this is an instance of @link BrInst @endlink
	IS_TYPE_METHOD(br, BR);

	/// returns `true` if this is an instance of @link CondBrInst @endlink
	IS_TYPE_METHOD(condbr, CONDBR);

	#undef IS_TYPE_METHOD
	#pragma endregion

	/// @copydoc Instruction::generate_ir()
	virtual String generate_ir() = 0;
};

#pragma region BranchInst subclasses
#define MEMBERS(name, type, term) \
	private: static constexpr const char* ir_name = name; \
	static const BranchInstType branch_inst_type = type; \
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