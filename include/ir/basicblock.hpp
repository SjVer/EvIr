//===--- ir/basicblock.hpp -------------- C++ ---===
// This header is part of the EvIr library
// MIT License - Sjoerd Vermeulen (2022)
// For more info see https://github.com/SjVer/EvIr
//===--------------------------------------------===

#ifndef EVIR_IR_BASICBLOCK_H
#define EVIR_IR_BASICBLOCK_H
#define __EVIR_HEADER

#include "evir/.common.hpp"
#include "evir/ir/instructions/instruction.hpp"

namespace evir {

class Function;

/// @brief A single basic block containing instructions.
///
/// A basic block is simply a container of instructions that execute sequentially.
/// Proper basic blocks should have a list of non-terminating instructions 
/// followed by a single terminator instruction. Terminator instructions may not
/// occur in the middle of a basic block, and must terminate the block.
/// This is not enforced during the "construction" of the IR.
class BasicBlock
{
	friend class Function;
	friend class IRBuilder;

	Function* parent;

	bool label_set;
	String label;
	std::function<String()> tmp_label_getter;
	
	Vector<BasicBlock*> preds;

	Vector<Instruction*> instructions;

public:

	BasicBlock() {}
	BasicBlock(String label, Function* parent = nullptr);

	Function* get_parent() { return parent; }

	void set_label(String label);
	String get_label();
	bool has_label() const { return label_set; }
	void remove_label() { label_set = false; }

	/// Checks if the block ends with a terminator instruction
	bool has_terminator() const { return instructions.back()->is_terminator(); }

	/// Adds a predecessing block
	void add_predecessor(BasicBlock* pred);

	/// Generates the IR for the block
	/// @return the IR as a string (with a newline)
	String generate_ir();
};

}

#endif // EVIR_IR_BASICBLOCK_H