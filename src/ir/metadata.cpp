#include "ir/metadata.hpp"
#include "ir/module.hpp"

using namespace eviir;

/// @section Constructors

Metadata::Metadata(BuiltinProperty type, Value* value): property_type(type), property_value(value) 
{
	switch(property_type)
	{
		// #define CASE(type, takes_value, ...) case type: \
		// 	ASSERT(takes_value ? (bool)value : !(bool)value, "property " STRINGIFY(type) " does not take a value!"); \
		// 	property_path = vector<string>{__VA_ARGS__}; break
		#define CASE(type, ...) case type: property_path = vector<string>{__VA_ARGS__}; break

		CASE(META_MODULE_NAME, "module", "name");
		CASE(META_MODULE_ENTRYPOINT, "module", "entrypoint");

		#undef CASE
		default: property_path = vector<string>{"<no path>"};
	}
	property_value = value;
}

Metadata::Metadata(vector<string> path, Value* value): property_value(value)
{
	property_type = META_CUSTOM;
	property_path = vector<string>(path);
}

/// @section IR Generation

string Metadata::generate_ir()
{
	/*
	ASSERT(property_type >= 0 && property_type < META_NONE, tools::fstr("Invalid metadata type '%d'!", property_type));
	
	Value* value;
	switch(property_type)
	{
		#define CASE(type, val) case type: value = val; break
	
		CASE(META_MODULE_NAME, new StringValue(module->name));
		CASE(META_CUSTOM, property_value);
	
		#undef CASE
		default: ASSERT(false, "Invalid metadata type!");
	}
	*/

	sstream stream = sstream();
	stream << "!";
	
	for(string& segment : property_path)
		stream << segment << (segment != property_path.back() ? "/" : "");
	
	if(property_value) stream << " " << property_value->generate_ir() << std::endl;
	else stream << " <no value>" << std::endl;
	
	return stream.str();
}