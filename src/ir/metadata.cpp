#include "ir/metadata.hpp"
#include "ir/module.hpp"

using namespace eviir;

Metadata::path Metadata::create_path(string full_path)
{
	// just split the string
	// a correct path is the user's responsibility
	return (path)tools::split_string(full_path, "/");
}

#pragma endregion
#pragma region Constructors

Metadata::Metadata(builtin_property_type type, Value* value)
{
	p_type = (property_type)type;

	switch(p_type)
	{
		#define CASE(type, ...) case type: p_path = vector<string>{__VA_ARGS__}; break

		CASE(META_MODULE_NAME, "module", "name");
		CASE(META_MODULE_ENTRYPOINT, "module", "entrypoint");

		#undef CASE
		default: ASSERT(0, "Invalid built-in metadata property type '" STRINGIFY(type) "'!");
	}
	

	p_value = value;
}

Metadata::Metadata(vector<string> path, Value* value)
{
	p_type = META_CUSTOM_;
	p_path = vector<string>(path);
	p_value = value;
}

#pragma endregion
#pragma region IR Generation

string Metadata::generate_ir()
{
	sstream stream = sstream();
	stream << "!";
	
	for(string& segment : p_path)
		stream << segment << (segment != p_path.back() ? "/" : "");
	
	if(p_value) stream << " " << p_value->generate_ir();
	else stream << " <no value>";
	
	return stream.str();
}