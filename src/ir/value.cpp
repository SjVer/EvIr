#include "ir/value.hpp"

using namespace eviir;

/// @section Constructors

StringValue* Value::new_string(string value)
{
	return new StringValue(value);
}

/// @section IR Generation

string StringValue::generate_ir()
{
	return '"' + tools::escstr(value) + '"';
}