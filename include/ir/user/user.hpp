//===--- ir/user/user.hpp --------------- C++ ---===
// This header is part of the EvIr library
// MIT License - Sjoerd Vermeulen (2022)
// For more info see https://github.com/SjVer/EvIr
//===--------------------------------------------===

#ifndef EVIR_IR_USER_USER_H
#define EVIR_IR_USER_USER_H

#include "evir/.common.hpp"
#include "evir/ir/type.hpp"

namespace evir {

class Module;

class User
{
	friend class Module;
	
protected:

	String name;
	Type* type;
	Vector<String> properties;

	Module* parent;

	String generate_props_comment();

public:

	/// Gets the name of the user
	String get_name() { return name; };

	Module* get_parent() const { return parent; }

	/// Adds a property to the user
	void add_property(String property) { properties.push_back(property); }

	virtual bool is_defined() const = 0;

	/// Generates the IR for the user
	/// @return the IR as a string (without a newline)
	virtual String generate_ir() = 0;
};

}

#endif // EVIR_IR_USER_USER_H