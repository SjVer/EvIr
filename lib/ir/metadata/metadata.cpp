#include "ir/metadata/metadata.hpp"
#include "ir/module.hpp"

using namespace evir;

// const char* Metadata::property_id_formats[_MD_none] = {
// 	#define FMT(type, fmt) [Metadata::_MD_##type] = fmt
//
// 	FMT(DEBUG_SOURCECHECKSUM, "0x%x"),
//
// 	#undef FMT
// };

Metadata::Path Metadata::create_path(String full_path)
{
	// just split the string
	// a correct path is the user's responsibility
	return (Path)tools::split_string(full_path, "/");
}

#pragma region Constructors

Metadata::Metadata(BuiltinPropertyID type, MDValue* value)
{
	p_id = (PropertyID)type;

	switch(type)
	{
		#define CASE(type, ...) case type: p_path = Path{__VA_ARGS__}; break

		CASE(MD_MODULE_NAME, "module", "name");
		CASE(MD_MODULE_ENTRYPOINT, "module", "entrypoint");

		CASE(MD_MODULE_SOURCE_FILENAME, "module", "source", "filename");
		CASE(MD_MODULE_SOURCE_DIRECTORY, "module", "source", "directory");
		CASE(MD_MODULE_SOURCE_LANGUAGE, "module", "source", "language");

		CASE(MD_MODULE_PRODUCER_NAME, "module", "producer", "name");
		CASE(MD_MODULE_PRODUCER_VERSION, "module", "producer", "version");
		CASE(MD_MODULE_PRODUCER_TYPE, "module", "producer", "type");

		CASE(MD_TARGET_TRIPLE, "target", "triple");
		CASE(MD_TARGET_CPU, "target", "cpu");
		CASE(MD_TARGET_DATALAYOUT, "target", "datalayout");
		CASE(MD_TARGET_OPTIMIZATION, "target", "optimization");

		CASE(MD_DEBUG_GENERATE, "debug", "generate");
		CASE(MD_DEBUG_INCLUDESOURCE, "debug", "includesource");
		CASE(MD_DEBUG_SOURCELOCATION, "debug", "sourcelocation");
		CASE(MD_DEBUG_SOURCECHECKSUM, "debug", "sourcechecksum");
		CASE(MD_DEBUG_DWARFVERSION, "debug", "dwarfversion");

		CASE(MD_DEBUG_TYPENAMES, "debug", "typenames");

		#undef CASE
		default: ASSERT_F(0, "Invalid built-in metadata property type %d!", type);
	}

	p_value = value;
}

Metadata::Metadata(CustomPropertyID type, Path path, MDValue* value)
{
	p_id = (PropertyID)type;
	
	// find base path
	switch(type)
	{
		#define CASE(type, ...) case type: p_path = Path{__VA_ARGS__}; break

		CASE(MD_MODULE, "module");
		CASE(MD_MODULE_SOURCE, "module", "source");
		CASE(MD_MODULE_PRODUCER, "module", "producer");
		CASE(MD_TARGET, "target");
		CASE(MD_DEBUG, "debug");
		CASE(MD_CUSTOM, );

		#undef CASE
		default: ASSERT_F(0, "Invalid custom metadata property type %d!", type);
	}

	// concat paths
	p_path.insert(p_path.end(), path.begin(), path.end());

	p_value = value;
}

#pragma endregion

String Metadata::generate_ir()
{
	SStream stream = SStream();
	stream << "!";
	
	for(String& segment : p_path)
		stream << segment << (segment != p_path.back() ? "/" : "");
	
	// if(p_value) stream << " " << p_value->generate_ir(property_id_formats[p_id]);
	if(p_value) stream << " " << p_value->generate_ir();
	else stream << " <no value>";
	
	return stream.str();
}