//===--- ir/user/function.hpp ----------- C++ ---===
// This header is part of the EvIr library
// MIT License - Sjoerd Vermeulen (2022)
// For more info see https://github.com/SjVer/EvIr
//===--------------------------------------------===

#ifndef EVIR_IR_USER_FUNCTION_H
#define EVIR_IR_USER_FUNCTION_H

#include ".common.hpp"
#include "ir/user/user.hpp"
#include "ir/basicblock.hpp"

namespace evir {

class Function : public User
{

	Vector<BasicBlock*> blocks;

public:

	Function(FunctionType* type, String name);

	/// Appends a new @link BasicBlock @endlink to the function's body
	void append_block(BasicBlock* block);

	/// @copydoc User::generate_ir()
	String generate_ir();
};

}

#endif // EVIR_IR_USER_FUNCTION_H