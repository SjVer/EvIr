//===---------- ir/metadata.hpp --- C++ ---------===
// This header is part of the EvIr library
// MIT License - Sjoerd Vermeulen (2022)
// For more info see https://github.com/SjVer/EvIr
//===--------------------------------------------===

#ifndef EVIR_IR_METADATA_H
#define EVIR_IR_METADATA_H

#include ".common.hpp"
#include "ir/value.hpp"

namespace evir
{

/// @brief A class defining the metadata of a @link Module @endlink.
///
/// Can be added to a module using @link Module::add_metadata @endlink.
class Metadata
{
	friend class Module;

	typedef enum
	{
		_META_start_module,
		_META_MODULE_NAME,
		_META_MODULE_ENTRYPOINT,
		_META_MODULE_,
		_META_end_module,

		_META_start_module_source,
		_META_MODULE_SOURCE_FILENAME,
		_META_MODULE_SOURCE_DIRECTORY,
		_META_MODULE_SOURCE_LANGUAGE,
		_META_MODULE_SOURCE_,
		_META_end_module_source,

		_META_start_module_producer,
		_META_MODULE_PRODUCER_NAME,
		_META_MODULE_PRODUCER_VERSION,
		_META_MODULE_PRODUCER_TYPE,
		_META_MODULE_PRODUCER_,
		_META_end_module_producer,

		_META_start_target,
		_META_TARGET_TRIPLE,
		_META_TARGET_CPU,
		_META_TARGET_DATALAYOUT,
		_META_TARGET_OPTIMIZATION,
		_META_TARGET_,
		_META_end_target,

		_META_start_debug,
		_META_DEBUG_GENERATE,
		_META_DEBUG_INCLUDESOURCE,
		_META_DEBUG_SOURCELOCATION,
		_META_DEBUG_SOURCECHECKSUM,
		_META_DEBUG_DWARFVERSION,
		_META_DEBUG_TYPENAMES,
		_META_DEBUG_,
		_META_end_debug,

		_META_CUSTOM_,

		_META_none,
	} property_type;

	static const char* property_type_formats[_META_none];

public:

	#pragma region types

	/// A metadata path
	typedef vector<string> path;

	/// @brief built-in metadata property types.
	/// @details Used by @link Metadata::(builtin_property_type type, Value* value = nullptr) @endlink.
	typedef enum
	{
		#define META(name) META_##name = property_type::_META_##name

		META(MODULE_NAME), 				///< metadata property path: `!module/name`
		META(MODULE_ENTRYPOINT), 		///< metadata property path: `!module/entrypoint`

		META(MODULE_SOURCE_FILENAME), 	///< metadata property path: `!module/source/filename`
		META(MODULE_SOURCE_DIRECTORY), 	///< metadata property path: `!module/source/directory`
		META(MODULE_SOURCE_LANGUAGE), 	///< metadata property path: `!module/source/language`

		META(MODULE_PRODUCER_NAME), 	///< metadata property path: `!module/producer/name`
		META(MODULE_PRODUCER_VERSION), 	///< metadata property path: `!module/producer/version`
		META(MODULE_PRODUCER_TYPE), 	///< metadata property path: `!module/producer/type`

		META(TARGET_TRIPLE), 			///< metadata property path: `!target/triple`
		META(TARGET_CPU), 				///< metadata property path: `!target/cpu`
		META(TARGET_DATALAYOUT), 		///< metadata property path: `!target/datalayout`
		META(TARGET_OPTIMIZATION), 		///< metadata property path: `!target/optimization`

		META(DEBUG_GENERATE), 			///< metadata property path: `!debug/generate`
		META(DEBUG_INCLUDESOURCE), 		///< metadata property path: `!debug/includesource`
		META(DEBUG_SOURCELOCATION), 	///< metadata property path: `!debug/sourcelocation`
		META(DEBUG_SOURCECHECKSUM), 	///< metadata property path: `!debug/sourcechecksum`
		META(DEBUG_DWARFVERSION), 		///< metadata property path: `!debug/dwarfversion`
		META(DEBUG_TYPENAMES), 			///< metadata property path: `!debug/typenames`

		#undef META
	} builtin_property_type;
	
	/// @brief custom metadata property types.
	/// @details Used by @link Metadata::Metadata(custom_property_type type, path path, Value* value = nullptr) @endlink.
	typedef enum
	{
		#define META(name) META_##name = property_type::_META_##name##_

		META(MODULE),			///< metadata property path: `!module/...`
		META(MODULE_SOURCE),	///< metadata property path: `!module/source/...`
		META(MODULE_PRODUCER),	///< metadata property path: `!module/producer/...`
		META(TARGET),			///< metadata property path: `!target/...`
		META(DEBUG),			///< metadata property path: `!debug/...`
		META(CUSTOM),			///< metadata property path: `!...`

		#undef META
	} custom_property_type;

	#pragma endregion

private:

	property_type p_type;
	path p_path;
	Value* p_value;

public:

	/// @brief Creates a Metadata path
	/// @details Splits the given string into segments using `/` delimeters and returns that as a path
	/// @param full_path the path to use (e.g. "base/sub")
	static path create_path(string full_path);

	#pragma region Constructors

	/// Constructs a new metadata instance defining a built-in property
	/// @param type the built-in property type that this metadata defines
	/// @param value the value of this metadata property
	Metadata(builtin_property_type type, Value* value = nullptr);

	/// Constructs a new metadata instance defining a custom property
	/// @param type the built-in type that this metadata defines a property of
	/// @param path the rest of the path of the property that this metadata defines
	/// @param value the value of this metadata property
	Metadata(custom_property_type type, path path, Value* value = nullptr);
	
	#pragma endregion

	/// Generates the IR for this metadata property
	/// @return the ir as a string (without a newline)
	string generate_ir();

	#pragma endregion
};

};

#endif // EVIR_IR_METADATA_H