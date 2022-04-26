#include "ir/irbuilder.hpp"

using namespace evir;

IRBuilder::IRBuilder(BasicBlock* bblock): bblock(bblock) {}

#pragma region IR Manipulation

Instruction* IRBuilder::insert(Instruction* inst)
{
	bblock->instructions.push_back(inst);
	return inst;
}

#pragma endregion