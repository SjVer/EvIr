//===--- ir/instructions/storage.hpp ---- C++ ---===
// This header is part of the EvIr library
// MIT License - Sjoerd Vermeulen (2022)
// For more info see https://github.com/SjVer/EvIr
//===--------------------------------------------===

#ifndef EVIR_IR_INSTRUCTIONS_STORAGE_H
#define EVIR_IR_INSTRUCTIONS_STORAGE_H
#define __EVIR_HEADER

#include "evir/.common.hpp"
#include "evir/ir/instructions/instruction.hpp"
#include "evir/ir/value/value.hpp"

namespace evir {

class StorageInst : public Instruction
{
protected:
	/// @cond

	static const char ir_prefix = '=';

	virtual void resolve() = 0;

	/// @endcond
public:

	/// @copydoc Instruction::generate_ir()
	virtual String generate_ir() = 0;
};

#pragma region StorageInst subclasses
#define MEMBERS(name, type, term) \
	private: void resolve(); \
	static const InstType inst_type = type;\
	static constexpr const char* ir_name = name; \
	public: bool is_terminator() const { return term; } \
	String generate_ir();

class DispInst : public StorageInst
{
	Value* expression;
	MEMBERS("disp", INST_DISP, false);
	
	DispInst(Value* expression): expression(expression) {}
};


#undef MEMBERS
#pragma endregion

}

#endif // EVIR_IR_INSTRUCTIONS_STORAGE_H