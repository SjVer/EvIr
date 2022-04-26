#include "ir/instructions/branch.hpp"

using namespace evir;

#pragma region Constructors

#define SIMPLE_CONSTRUCTOR(classname, lowercasename, membertype, membername) \
	classname* BranchInst::new_##lowercasename##_inst(membertype membername) \
	{ return new classname(membername); }

SIMPLE_CONSTRUCTOR(RetInst, ret, Value*, retval);

#undef SIMPLE_CONSTRUCTOR

#pragma endregion
#pragma region IR Generation
#define GENERATE_IR(classname) String classname::generate_ir()

GENERATE_IR(RetInst)
{
	//
	return _INST_IR_START + retval->generate_ir() + endl;
}

#pragma endregion