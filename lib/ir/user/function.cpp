#include "evir/ir/user/function.hpp"

using namespace evir;

Function::Function(FunctionType* type_, String name_)
{
	name = name_;
	type = type_;
}

void Function::append_block(BasicBlock* block)
{
	ASSERT(block, "invalid basic block!");
	block->parent = this;
	blocks.push_back(block);
}

String Function::generate_ir()
{
	SStream stream = SStream();

	// property comment
	stream << generate_props_comment();

	// function signature
	ASSERT(type, "function type is null!");
	ASSERT(name.length(), "function name is empty!");
	stream << name << " " << type->generate_ir() << endl;

	// set tmp labels where needed
	uint unnamed_blocks_count = 0;
	auto callback = [&]() -> String { return std::to_string(unnamed_blocks_count++); };
	for(auto block : blocks) block->tmp_label_getter = callback;

	// basicblocks (function body)
	for(auto block : blocks)
	{
		stream << block->generate_ir();
		if(block != blocks.back()) stream << endl;
	}

	return stream.str();
}