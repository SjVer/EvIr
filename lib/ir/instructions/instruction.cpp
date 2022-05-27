#include "evir/ir/instructions/instruction.hpp"
#include "evir/ir/basicblock.hpp"
#include "evir/ir/user/function.hpp"
#include "evir/ir/comment.hpp"

using namespace evir;

Function* Instruction::get_function() const
{
	//
	return get_parent()->get_parent();
}

Module* Instruction::get_module() const
{
	//
	return get_function()->get_parent();
}

String _comment_inst::_CommentInst::generate_ir()
{
	return generate_ir_comment(text, false);
}