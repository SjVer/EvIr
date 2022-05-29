#include "evir/ir/irbuilder.hpp"

using namespace evir;

IRBuilder::IRBuilder(BasicBlock* bblock): bblock(bblock) {}

#pragma region IR Manipulation

Instruction* IRBuilder::insert(Instruction* inst)
{
	inst->parent = bblock;
	bblock->instructions.push_back(inst);
	return inst;
}

void IRBuilder::insert_comment(String text)
{
	for(String s : tools::split_string(text, "\n"))
		bblock->instructions.push_back(new _comment_inst::_CommentInst(s));
}

#pragma endregion