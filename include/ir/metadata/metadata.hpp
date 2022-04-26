//===---------- ir/metadata.hpp --- C++ ---------===
// This header is part of the EvIr library
// MIT License - Sjoerd Vermeulen (2022)
// For more info see https://github.com/SjVer/EvIr
//===--------------------------------------------===

#ifndef EVIR_IR_METADATA_H
#define EVIR_IR_METADATA_H

#include "evir/.common.hpp"
#include "evir/ir/metadata/mdvalue.hpp"

namespace evir
{

/// @brief A class defining the metadata of a @link Module @endlink
///
/// Can be added to a module using @link Module::add_metadata @endlink
class Metadata
{
	friend class Module;

	typedef enum
	{
		_MD_start_module,
		_MD_MODULE_NAME,
		_MD_MODULE_ENTRYPOINT,
		_MD_MODULE_,
		_MD_end_module,

		_MD_start_module_source,
		_MD_MODULE_SOURCE_FILENAME,
		_MD_MODULE_SOURCE_DIRECTORY,
		_MD_MODULE_SOURCE_LANGUAGE,
		_MD_MODULE_SOURCE_,
		_MD_end_module_source,

		_MD_start_module_producer,
		_MD_MODULE_PRODUCER_NAME,
		_MD_MODULE_PRODUCER_VERSION,
		_MD_MODULE_PRODUCER_TYPE,
		_MD_MODULE_PRODUCER_,
		_MD_end_module_producer,

		_MD_start_target,
		_MD_TARGET_TRIPLE,
		_MD_TARGET_CPU,
		_MD_TARGET_DATALAYOUT,
		_MD_TARGET_OPTIMIZATION,
		_MD_TARGET_,
		_MD_end_target,

		_MD_start_debug,
		_MD_DEBUG_GENERATE,
		_MD_DEBUG_INCLUDESOURCE,
		_MD_DEBUG_SOURCELOCATION,
		_MD_DEBUG_SOURCECHECKSUM,
		_MD_DEBUG_DWARFVERSION,
		_MD_DEBUG_TYPENAMES,
		_MD_DEBUG_,
		_MD_end_debug,

		_MD_CUSTOM_,

		_MD_none,
	} PropertyID;

	// static const char* property_id_formats[_none];

public:

	#pragma region types

	/// A metadata path
	typedef Vector<String> Path;

	/// @brief built-in metadata property types
	/// @details Used by @link Metadata::Metadata(BuiltinPropertyID type, MDValue* value) @endlink
	typedef enum
	{
		#define META(name) MD_##name = PropertyID::_MD_##name

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
	} BuiltinPropertyID;
	
	/// @brief custom metadata property types
	/// @details Used by @link Metadata::Metadata(CustomPropertyID type, Path path, MDValue* value) @endlink
	typedef enum
	{
		#define META(name) MD_##name = PropertyID::_MD_##name##_

		META(MODULE),			///< metadata property path: `!module/...`
		META(MODULE_SOURCE),	///< metadata property path: `!module/source/...`
		META(MODULE_PRODUCER),	///< metadata property path: `!module/producer/...`
		META(TARGET),			///< metadata property path: `!target/...`
		META(DEBUG),			///< metadata property path: `!debug/...`
		META(CUSTOM),			///< metadata property path: `!...`

		#undef META
	} CustomPropertyID;

	#pragma endregion

private:

	PropertyID p_id;
	Path p_path;
	MDValue* p_value;

public:

	/// @brief Creates a Metadata path
	/// @details Splits the given string into segments using `/` delimeters and returns that as a path
	/// @param full_path the path to use (e.g. "base/sub")
	static Path create_path(String full_path);

	#pragma region Constructors

	/// @brief Constructs a new metadata instance defining a built-in property
	/// @param type the built-in property type that this metadata defines
	/// @param value the value of this metadata property
	Metadata(BuiltinPropertyID type, MDValue* value = nullptr);

	/// @brief Constructs a new metadata instance defining a custom property
	/// @param type the built-in type that this metadata defines a property of
	/// @param path the rest of the path of the property that this metadata defines
	/// @param value the value of this metadata property
	Metadata(CustomPropertyID type, Path path, MDValue* value = nullptr);
	
	#pragma endregion

	/// @brief Generates the IR for this metadata property
	/// @return the ir as a string (without a newline)
	String generate_ir();

	#pragma endregion
};

};

#endif // EVIR_IR_METADATA_H