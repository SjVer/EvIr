//===---------- ir/module.hpp --- C++ -----------===
// This header is part of the EvIr library 		
// MIT License - Sjoerd Vermeulen (2022)			
// For more info see https://github.com/SjVer/EvIr	
//===--------------------------------------------===

#ifndef EVIR_IR_MODULE_H
#define EVIR_IR_MODULE_H

#include ".common.hpp"
#include "ir/metadata.hpp"

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

	#pragma endregion
	#pragma region Private members

	string name;
	vector<Metadata*> metadata;

public:

	#pragma endregion
	#pragma region Constructors

	/// Constructs a new module
	/// @param module_name the name of the module
	Module(string module_name);

	#pragma endregion
	#pragma region Metadata manipulation

	/// Checks if the module has metadata with the given path
	bool has_metadata(Metadata::path path);

	/// Checks if the module has metadata with the given type
	bool has_metadata(Metadata::builtin_property_type type);

	/// Adds metadata to the module
	/// @param mdata the metadata to add
	/// @warning if the module already has metadata with the same path the function will abort
	void add_metadata(Metadata* mdata);

	/// Sets a metadata property with the given path
	/// @param path the path of the metadata property
	/// @param value the value to set the property to
	/// @warning if the module doesn't have metadata with the given path the function will abort
	void set_metadata(Metadata::path path, Value* value);

	/// Sets a metadata property with the built-in type
	/// @param type the type of the metadata property
	/// @param value the value to set the property to
	/// @warning if the module doesn't have metadata with the given type the function will abort
	void set_metadata(Metadata::builtin_property_type type, Value* value);

	/// Gets the requested metadata property
	/// @param path the path of the metadata property
	/// @return a pointer to the metadata, or null if it isn't found
	Metadata* get_metadata(Metadata::path path);

	/// Gets the requested metadata property
	/// @param type the built-in type of the metadata property
	/// @return a pointer to the metadata, or null if it isn't found
	Metadata* get_metadata(Metadata::builtin_property_type type);

	#pragma endregion
	#pragma region IR generation

	/// Generates an IR comment
	/// @param text the text of the comment
	/// @param header if true, the comment is formatted as a header comment
	/// @return the IR as a string (with newline)
	string generate_ir_comment(string text, bool header = false);

	/// Generates the IR of the current module's metadata.
	/// If before_contents is true, the metadata that comes before 
	/// the module's is generated, otherwise the metadata that
	/// comes after the module's contents is generated
	/// @param before_contents see description
	/// @return the IR as a string (with newline)
	string generate_metadata_ir(bool before_contents);

	/// Generates the IR of the current module
	/// @return the IR as a string (with newline)
	string generate_ir();

	#pragma endregion
};

}

#endif // EVIR_IR_MODULE_H