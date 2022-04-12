#include "ir/metadata.hpp"
#include "ir/module.hpp"

using namespace evir;

const char* Metadata::property_type_formats[_META_none] = {
	#define FMT(type, fmt) [Metadata::_META_##type] = fmt

	FMT(DEBUG_SOURCECHECKSUM, "0x%x"),
	
	#undef FMT
};

Metadata::path Metadata::create_path(string full_path)
{
	// just split the string
	// a correct path is the user's responsibility
	return (path)tools::split_string(full_path, "/");
}

#pragma region Constructors

Metadata::Metadata(builtin_property_type type, Value* value)
{
	p_type = (property_type)type;

	switch(type)
	{
		#define CASE(type, ...) case type: p_path = vector<string>{__VA_ARGS__}; break

		CASE(META_MODULE_NAME, "module", "name");
		CASE(META_MODULE_ENTRYPOINT, "module", "entrypoint");

		CASE(META_MODULE_SOURCE_FILENAME, "module", "source", "filename");
		CASE(META_MODULE_SOURCE_DIRECTORY, "module", "source", "directory");
		CASE(META_MODULE_SOURCE_LANGUAGE, "module", "source", "language");

		CASE(META_MODULE_PRODUCER_NAME, "module", "producer", "name");
		CASE(META_MODULE_PRODUCER_VERSION, "module", "producer", "version");
		CASE(META_MODULE_PRODUCER_TYPE, "module", "producer", "type");

		CASE(META_TARGET_TRIPLE, "target", "triple");
		CASE(META_TARGET_CPU, "target", "cpu");
		CASE(META_TARGET_DATALAYOUT, "target", "datalayout");
		CASE(META_TARGET_OPTIMIZATION, "target", "optimization");

		CASE(META_DEBUG_GENERATE, "debug", "generate");
		CASE(META_DEBUG_INCLUDESOURCE, "debug", "includesource");
		CASE(META_DEBUG_SOURCELOCATION, "debug", "sourcelocation");
		CASE(META_DEBUG_SOURCECHECKSUM, "debug", "sourcechecksum");
		CASE(META_DEBUG_DWARFVERSION, "debug", "dwarfversion");

		CASE(META_DEBUG_TYPENAMES, "debug", "typenames");

		#undef CASE
		default: ASSERT_F(0, "Invalid built-in metadata property type %d!", type);
	}
	

	p_value = value;
}

Metadata::Metadata(custom_property_type type, vector<string> path, Value* value)
{
	p_type = (property_type)type;
	
	// find base path
	switch(type)
	{
		#define CASE(type, ...) case type: p_path = vector<string>{__VA_ARGS__}; break

		CASE(META_MODULE, "module");
		CASE(META_MODULE_SOURCE, "module", "source");
		CASE(META_MODULE_PRODUCER, "module", "producer");
		CASE(META_TARGET, "target");
		CASE(META_DEBUG, "debug");
		CASE(META_CUSTOM, );

		#undef CASE
		default: ASSERT_F(0, "Invalid custom metadata property type %d!", type);
	}

	// concat paths
	p_path.insert(p_path.end(), path.begin(), path.end());

	p_value = value;
}

#pragma endregion

string Metadata::generate_ir()
{
	sstream stream = sstream();
	stream << "!";
	
	for(string& segment : p_path)
		stream << segment << (segment != p_path.back() ? "/" : "");
	
	if(p_value) stream << " " << p_value->generate_ir(property_type_formats[p_type]);
	else stream << " <no value>";
	
	return stream.str();
}