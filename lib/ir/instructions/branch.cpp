#include "ir/instructions/branch.hpp"

using namespace evir;

String RetInst::generate_ir()
{
	//
	return _INST_IR_START + retval->generate_ir() + endl;
}
