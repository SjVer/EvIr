#include "evir/ir/instructions/storage.hpp"

using namespace evir;

#pragma region Resolving

void DispInst::resolve()
{
	// expression->resolve();
};

#pragma endregion
#pragma region IR Generation

String DispInst::generate_ir()
{
	//
	return __EVIR_INST_IR_START + expression->generate_ir() + endl;
}

#pragma endregion