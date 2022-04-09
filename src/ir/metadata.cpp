#include "ir/metadata.hpp"
#include "ir/module.hpp"

using namespace eviir;

Metadata::Metadata(BuiltinProperty property): property_type(property) 
{
	switch(property)
	{
		#define CASE(type, ...) case type: property_path = vector<string>((string[]){__VA_ARGS__}); break

		CASE(META_MODULE_NAME, "module", "name");

		#undef CASE
		default: property_path = vector<string>("<no path>");
	}
}

string Metadata::generate_ir(Module* module)
{
	ASSERT(property_type >= 0 && property_type < META_NONE, "Invalid metadata type!");

	// Value* value;
	string value;

	switch(property_type)
	{
		#define CASE(type, val) case type: value = val; break

		CASE(META_MODULE_NAME, module->name);
		CASE(META_CUSTOM, "<no value>" /* property_value->generate_ir() */);

		#undef CASE
		default: ASSERT(false, "Invalid metadata type!");
	}

	sstream stream = sstream("!");
	for(string& segment : property_path)
		stream << segment << (segment != property_path.back() ? "/" : "");
	
	stream << " " << value /* value->generate_ir() */ << std::endl;
	return stream.str();
}