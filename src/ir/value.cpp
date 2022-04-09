#include "ir/value.hpp"

using namespace eviir;

/// @section Constructors

StringValue* Value::new_string(string value)
{
	return new StringValue(value);
}

ReferenceValue* Value::new_reference(string name)
{
	return new ReferenceValue(name);
}

/// @section IR Generation

string StringValue::generate_ir()
{
	return '"' + tools::escstr(value) + '"';
}

string ReferenceValue::generate_ir()
{
	return '%' + name;
}