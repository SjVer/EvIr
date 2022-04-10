//===---------- ir/metadata.hpp --- C++ ---------===
// This header is part of the EviIr library
// MIT License - Sjoerd Vermeulen (2022)
// For more info see https://github.com/SjVer/EviIr
//===--------------------------------------------===

#ifndef EVIIR_IR_METADATA_H
#define EVIIR_IR_METADATA_H

#include ".common.hpp"
#include "ir/value.hpp"

namespace eviir
{

/// A Class defining metadata for modules
class Metadata
{
	friend class Module;

	typedef enum
	{
		_META_start_module,
		_META_MODULE_NAME,				// module/name
		_META_MODULE_ENTRYPOINT,		// module/entrypoint
		_META_MODULE_,					// module/...
		_META_end_module,

		_META_start_module_source,
		_META_MODULE_SOURCE_FILENAME,	// module/source/filename
		_META_MODULE_SOURCE_DIRECTORY,	// module/source/directory
		_META_MODULE_SOURCE_LANGUAGE,	// module/source/language
		_META_MODULE_SOURCE_,			// module/source/...
		_META_end_module_source,

		_META_start_module_producer,
		_META_MODULE_PRODUCER_NAME,		// module/producer/name
		_META_MODULE_PRODUCER_VERSION,	// module/producer/version
		_META_MODULE_PRODUCER_TYPE,		// module/producer/type
		_META_MODULE_PRODUCER_,			// module/producer/...
		_META_end_module_producer,

		_META_start_target,
		_META_TARGET_TRIPLE,			// target/triple
		_META_TARGET_CPU,				// target/cpu
		_META_TARGET_DATALAYOUT,		// target/datalayout
		_META_TARGET_OPTIMIZATION,		// target/optimization
		_META_TARGET_,					// target/...
		_META_end_target,

		_META_start_debug,
		_META_DEBUG_GENERATE,			// debug/generate
		_META_DEBUG_INCLUDESOURCE,		// debug/includesource
		_META_DEBUG_SOURCELOCATION,		// debug/sourcelocation
		_META_DEBUG_SOURCECHECKSUM,		// debug/sourcechecksum
		_META_DEBUG_DWARFVERSION,		// debug/dwarfversion
		_META_DEBUG_TYPENAMES,			// debug/typenames
		_META_DEBUG_,					// debug/...
		_META_end_debug,

		_META_CUSTOM_,					// ...

		_META_none,
	} property_type;

	static const char* property_type_formats[_META_none];

public:

	#pragma region types

	/// A metadata path
	typedef vector<string> path;

	/// The built-in metadata property types
	typedef enum
	{
		#define META(name) META_##name = property_type::_META_##name

		META(MODULE_NAME),
		META(MODULE_ENTRYPOINT),

		META(MODULE_SOURCE_FILENAME),
		META(MODULE_SOURCE_DIRECTORY),
		META(MODULE_SOURCE_LANGUAGE),

		META(MODULE_PRODUCER_NAME),
		META(MODULE_PRODUCER_VERSION),
		META(MODULE_PRODUCER_TYPE),

		META(TARGET_TRIPLE),
		META(TARGET_CPU),
		META(TARGET_DATALAYOUT),
		META(TARGET_OPTIMIZATION),

		META(DEBUG_GENERATE),
		META(DEBUG_INCLUDESOURCE),
		META(DEBUG_SOURCELOCATION),
		META(DEBUG_SOURCECHECKSUM),
		META(DEBUG_DWARFVERSION),
		META(DEBUG_TYPENAMES),

		#undef META
	} builtin_property_type;
	
	/// The custom metadata property types
	typedef enum
	{
		#define META(name) META_##name = property_type::_META_##name##_

		META(MODULE),
		META(MODULE_SOURCE),
		META(MODULE_PRODUCER),
		META(TARGET),
		META(DEBUG),
		META(CUSTOM),

		#undef META
	} custom_property_type;

	#pragma endregion

private:

	property_type p_type;
	path p_path;
	Value* p_value;

public:

	/// Creates a Metadata path
	/// @param full_path the path to use (e.g. "base/sub")
	static path create_path(string full_path);

	#pragma region Constructors

	/// Constructs a new metadata instance defining a built-in property
	/// @param type the built-in property type that this metadata defines
	/// @param value the value of this metadata property (optional)
	Metadata(builtin_property_type type, Value* value = nullptr);

	/// Constructs a new metadata instance defining a custom property
	/// @param type the built-in type that this metadata defines a property of
	/// @param path the rest of the path of the property that this metadata defines
	/// @param value the value of this metadata property (optional)
	Metadata(custom_property_type type, path path, Value* value = nullptr);
	
	#pragma endregion

	/// Generates the IR for this metadata property
	/// @return the ir as a string (without a newline)
	string generate_ir();

	#pragma endregion
};

};

#endif // EVIIR_IR_METADATA_H