//===--- ir/user/function.hpp ----------- C++ ---===
// This header is part of the EvIr library
// MIT License - Sjoerd Vermeulen (2022)
// For more info see https://github.com/SjVer/EvIr
//===--------------------------------------------===

#ifndef EVIR_IR_USER_FUNCTION_H
#define EVIR_IR_USER_FUNCTION_H
#define __EVIR_HEADER

#include "evir/.common.hpp"
#include "evir/ir/user/user.hpp"
#include "evir/ir/basicblock.hpp"

namespace evir {

class Function : public User
{
	Vector<BasicBlock*> blocks;

public:

	Function(FunctionType* type, String name);

	/// Appends a new @link BasicBlock @endlink to the function's body
	void append_block(BasicBlock* block);

	Type* get_return_type() const { return ((FunctionType*)type)->return_type; }
	Vector<Type*> get_params() const { return ((FunctionType*)type)->params; }
	size get_param_count() const { return get_params().size(); }
	bool is_defined() const { return blocks.size() > 0; };

	

	/// @copydoc User::generate_ir()
	String generate_ir();
};

}

#endif // EVIR_IR_USER_FUNCTION_H