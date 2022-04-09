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

	enum class property_type
	{
		_META_start_module,
		META_MODULE_,					// module/...
		META_MODULE_NAME,				// module/name
		META_MODULE_ENTRYPOINT,			// module/entrypoint
		_META_end_module,

		_META_start_module_source,
		META_MODULE_SOURCE_,			// module/source/...
		META_MODULE_SOURCE_FILENAME,	// module/source/filename
		META_MODULE_SOURCE_DIRECTORY,	// module/source/directory
		META_MODULE_SOURCE_LANGUAGE,	// module/source/language
		_META_end_module_source,

		_META_start_module_producer,
		META_MODULE_PRODUCER_,			// module/producer/...
		META_MODULE_PRODUCER_NAME,		// module/producer/name
		META_MODULE_PRODUCER_VERSION,	// module/producer/version
		META_MODULE_PRODUCER_TYPE,		// module/producer/type
		_META_end_module_producer,

		_META_start_target,
		META_TARGET_,					// target/...
		META_TARGET_TRIPLE,				// target/triple
		META_TARGET_CPU,				// target/cpu
		META_TARGET_DATALAYOUT,			// target/datalayout
		META_TARGET_OPTIMIZATION,		// target/optimization
		_META_end_target,

		_META_start_debug,
		META_DEBUG_,					// debug/...
		META_DEBUG_GENERATE,			// debug/generate
		META_DEBUG_INCLUDESOURCE,		// debug/includesource
		META_DEBUG_SOURCELOCATION,		// debug/sourcelocation
		META_DEBUG_SOURCECHECKSUM,		// debug/sourcechecksum
		META_DEBUG_DWARFVERSION,		// debug/dwarfversion
		META_DEBUG_TYPENAMES,			// debug/typenames
		_META_end_debug,

		META_CUSTOM_,					// ...
		META_NONE,
	};

public:

// TODO: Typecast overload?

	/// A metadata path
	typedef vector<string> path;

	/// The built-in metadata property types
	typedef enum
	{
		#define META(name) META_##name = property_type::META_##name

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

		#undef METAs
	} builtin_property_type;
	
	/// The custom metadata property types
	typedef enum
	{
		#define META(name) META_##name = property_type::META_##name##_

		META(MODULE),
		META(MODULE_SOURCE),
		META(MODULE_PRODUCER),
		META(TARGET),
		META(DEBUG),
		META_CUSTOM = property_type::META_CUSTOM_,

		#undef META
	} custom_property_type;

private:

	property_type p_type;
	path p_path;
	Value* p_value;

public:

	/// Creates a Metadata path
	/// @param full_path the path to use (e.g. "base/sub")
	static path create_path(string full_path);

	#pragma endregion
	#pragma region Constructors

	/// Constructs a new metadata instance defining a built-in property
	/// @param property the built-in property that this metadata defines
	/// @param value the value of this metadata property (optional)
	Metadata(builtin_property_type p_type, Value* value = nullptr);

	/// Constructs a new metadata instance defining a custom property
	/// @param path the path of the property that this metadata defines
	/// @param value the value of this metadata property (optional)
	Metadata(path path, Value* value = nullptr);
	
	#pragma endregion
	#pragma region IR generation

	/// Generates the IR for this metadata property
	/// @return the ir as a string (without a newline)
	string generate_ir();
};

};

#endif // EVIIR_IR_METADATA_H