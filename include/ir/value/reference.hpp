//===--- ir/object/reference.hpp -------- C++ ---===
// This header is part of the EvIr library
// MIT License - Sjoerd Vermeulen (2022)
// For more info see https://github.com/SjVer/EvIr
//===--------------------------------------------===

#ifndef EVIR_IR_OBJECT_REFERENCE_H
#define EVIR_IR_OBJECT_REFERENCE_H

#include ".common.hpp"
#include "ir/value/value.hpp"

namespace evir {

class Reference : public Value
{
protected:
	/// @cond

	static const ValueType value_type = VALUE_REFERENCE;

	/// @endcond
public:

	#pragma region Constructors

	

	#pragma endregion
};

};

#endif // EVIR_IR_OBJECT_REFERENCE_H