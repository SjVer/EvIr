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
		/// module/...
		_META_MODULE_,					
		_META_end_module,

		_META_start_module_source,
		_META_MODULE_SOURCE_FILENAME,
		_META_MODULE_SOURCE_DIRECTORY,
		_META_MODULE_SOURCE_LANGUAGE,	
		/// module/source/...
		_META_MODULE_SOURCE_,			
		_META_end_module_source,

		_META_start_module_producer,
		/// module/producer/name
		_META_MODULE_PRODUCER_NAME,		
		/// module/producer/version
		_META_MODULE_PRODUCER_VERSION,	
		/// module/producer/type
		_META_MODULE_PRODUCER_TYPE,		
		/// module/producer/...
		_META_MODULE_PRODUCER_,			
		_META_end_module_producer,

		_META_start_target,
		/// target/triple
		_META_TARGET_TRIPLE,			
		/// target/cpu
		_META_TARGET_CPU,				
		/// target/datalayout
		_META_TARGET_DATALAYOUT,		
		/// target/optimization
		_META_TARGET_OPTIMIZATION,		
		/// target/...
		_META_TARGET_,					
		_META_end_target,

		_META_start_debug,
		/// debug/generate
		_META_DEBUG_GENERATE,			
		/// debug/includesource
		_META_DEBUG_INCLUDESOURCE,		
		/// debug/sourcelocation
		_META_DEBUG_SOURCELOCATION,		
		/// debug/sourcechecksum
		_META_DEBUG_SOURCECHECKSUM,		
		/// debug/dwarfversion
		_META_DEBUG_DWARFVERSION,		
		/// debug/typenames
		_META_DEBUG_TYPENAMES,			
		/// debug/...
		_META_DEBUG_,					
		_META_end_debug,

		/// ...
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

		META(MODULE_NAME), 				/// metadata property path: `!module/name`
		META(MODULE_ENTRYPOINT), 		/// metadata property path: `!module/entrypoint`

		META(MODULE_SOURCE_FILENAME), 	/// metadata property path: `!module/source/filename`
		META(MODULE_SOURCE_DIRECTORY), 	/// metadata property path: `!module/source/directory`
		META(MODULE_SOURCE_LANGUAGE), 	/// metadata property path: `!module/source/language`

		META(MODULE_PRODUCER_NAME), 	/// whoop 
		META(MODULE_PRODUCER_VERSION), 	/// whoop 
		META(MODULE_PRODUCER_TYPE), 	/// whoop 

		META(TARGET_TRIPLE), 			/// whoop 
		META(TARGET_CPU), 				/// whoop 
		META(TARGET_DATALAYOUT), 		/// whoop 
		META(TARGET_OPTIMIZATION), 		/// whoop 

		META(DEBUG_GENERATE), 			/// whoop 
		META(DEBUG_INCLUDESOURCE), 		/// whoop 
		META(DEBUG_SOURCELOCATION), 	/// whoop 
		META(DEBUG_SOURCECHECKSUM), 	/// whoop 
		META(DEBUG_DWARFVERSION), 		/// whoop 
		META(DEBUG_TYPENAMES), 			/// whoop 

		#undef META
	} builtin_property_type;
	
	/// @brief custom metadata property types.
	/// @details Used by @link Metadata::Metadata(custom_property_type type, path path, Value* value = nullptr) @endlink.
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