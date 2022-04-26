//===--- ir/value/operator.hpp ---------- C++ ---===
// This header is part of the EvIr library
// MIT License - Sjoerd Vermeulen (2022)
// For more info see https://github.com/SjVer/EvIr
//===--------------------------------------------===

#ifndef EVIR_IR_VALUE_OPERATOR_H
#define EVIR_IR_VALUE_OPERATOR_H

#include "evir/.common.hpp"
#include "evir/ir/value/value.hpp"

namespace evir {

class Operator : public Value
{
protected:
	/// @cond

	static const ValueType value_type = VALUE_OPERATOR;

	/// @endcond
public:

	#pragma region Constructors

	

	#pragma endregion
};

}

#endif // EVIR_IR_VALUE_OPERATOR_H