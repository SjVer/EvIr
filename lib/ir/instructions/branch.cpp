#include "evir/ir/instructions/branch.hpp"
#include "evir/ir/basicblock.hpp"

using namespace evir;

#pragma region Resolving

void RetInst::resolve() {};

void BrInst::resolve()
{
	if(dest) dest->add_predecessor(get_parent());
}

void CondBrInst::resolve()
{
	if(true_dest) true_dest->add_predecessor(get_parent());
	if(false_dest) false_dest->add_predecessor(get_parent());
}

#pragma endregion
#pragma region IR Generation

String RetInst::generate_ir()
{
	//
	return __EVIR_INST_IR_START + retval->generate_ir() + endl;
}

String BrInst::generate_ir()
{
	String d = dest ? dest->get_label() : UNRESOLVED;
	return __EVIR_INST_IR_START + '&' + d + endl;
}

String CondBrInst::generate_ir()
{
	String c = condition->generate_ir();
	String td = true_dest ? " &" + true_dest->get_label() : " &" UNRESOLVED;
	String fd = false_dest ? " &" + false_dest->get_label() : "";

	return __EVIR_INST_IR_START + c + td + fd + endl;
}

#pragma endregion