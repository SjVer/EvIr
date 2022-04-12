//===--- ir/module.hpp ------------------ C++ ---===
// This header is part of the EvIr library 		
// MIT License - Sjoerd Vermeulen (2022)			
// For more info see https://github.com/SjVer/EvIr	
//===--------------------------------------------===

#ifndef EVIR_IR_MODULE_H
#define EVIR_IR_MODULE_H

#include ".common.hpp"
#include "ir/metadata.hpp"
#include "ir/value/value.hpp"

#define __IR_COMMENT_LENGTH 49
#define __IR_HCOMMENT_LENGTH 50
#define __IR_HCOMMENT_SURROUND_CHAR '='
#define __IR_HCOMMENT_MIN_SURROUND 3

namespace evir
{

/// @brief A Class representing a single stand-alone IR module
///
/// Has the following metadata by default:
/// - @link Metadata::builtin_property_type @endlink::`META_MODULE_NAME` 
/// - @link Metadata::builtin_property_type @endlink::`META_MODULE_ENTRYPOINT` 
class Module
{
	friend class Metadata;

	String name;
	Vector<Metadata*> metadata;

public:

	#pragma region Constructors

	/// @brief Constructs a new module
	/// @param module_name the name of the module
	Module(String module_name);

	#pragma endregion
	#pragma region Metadata manipulation

	/// @brief Checks if the module has metadata with the given path
	bool has_metadata(Metadata::Path path);

	/// @brief Checks if the module has metadata with the given type
	bool has_metadata(Metadata::BuiltinPropertyType type);

	/// @brief Adds metadata to the module
	/// @param mdata the metadata to add
	/// @warning if the module already has metadata with the same path the function will abort
	void add_metadata(Metadata* mdata);

	/// @brief Sets a metadata property with the given path
	/// @param path the path of the metadata property
	/// @param value the value to set the property to
	/// @warning if the module doesn't have metadata with the given path the function will abort
	void set_metadata(Metadata::Path path, Value* value);

	/// @brief Sets a metadata property with the built-in type
	/// @param type the type of the metadata property
	/// @param value the value to set the property to
	/// @warning if the module doesn't have metadata with the given type the function will abort
	void set_metadata(Metadata::BuiltinPropertyType type, Value* value);

	/// @brief Gets the requested metadata property
	/// @param path the path of the metadata property
	/// @return a pointer to the metadata, or null if it isn't found
	Metadata* get_metadata(Metadata::Path path);

	/// @brief Gets the requested metadata property
	/// @param type the built-in type of the metadata property
	/// @return a pointer to the metadata, or null if it isn't found
	Metadata* get_metadata(Metadata::BuiltinPropertyType type);

	#pragma endregion
	#pragma region User manipulation

	

	#pragma endregion
	#pragma region IR generation

	/// @brief Generates an IR comment
	/// @param text the text of the comment
	/// @param header if true, the comment is formatted as a header comment
	/// @return the IR as a string (with newline)
	String generate_ir_comment(String text, bool header = false);

	/// @brief Generates the IR of the current module's metadata
	/// @details If before_contents is true, the metadata that comes before 
	/// the module's is generated, otherwise the metadata that
	/// comes after the module's contents is generated
	/// @param before_contents see description
	/// @return the IR as a string (with newline)
	String generate_metadata_ir(bool before_contents);

	/// @brief Generates the IR of the current module
	/// @return the IR as a string (with newline)
	String generate_ir();

	#pragma endregion
};

}

#endif // EVIR_IR_MODULE_H