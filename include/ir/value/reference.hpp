//===--- ir/value/reference.hpp --------- C++ ---===
// This header is part of the EvIr library
// MIT License - Sjoerd Vermeulen (2022)
// For more info see https://github.com/SjVer/EvIr
//===--------------------------------------------===

#ifndef EVIR_IR_VALUE_REFERENCE_H
#define EVIR_IR_VALUE_REFERENCE_H

#include "evir/.common.hpp"
#include "evir/ir/value/value.hpp"
#include "evir/ir/user/user.hpp"

namespace evir {

class Reference : public Value
{
protected:
	/// @cond

	static const ValueType value_type = VALUE_REFERENCE;
	User* user;

	/// @endcond
public:

	Reference(User* user = nullptr): user(user) {}

	/// @copydoc Value::generate_ir()
	String generate_ir();
};

};

#endif // EVIR_IR_VALUE_REFERENCE_H