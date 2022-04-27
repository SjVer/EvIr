//===--- ir/module.hpp ------------------ C++ ---===
// This header is part of the EvIr library 		
// MIT License - Sjoerd Vermeulen (2022)			
// For more info see https://github.com/SjVer/EvIr	
//===--------------------------------------------===

#ifndef EVIR_IR_MODULE_H
#define EVIR_IR_MODULE_H
#define __EVIR_HEADER

#include "evir/.common.hpp"
#include "evir/ir/metadata/metadata.hpp"
#include "evir/ir/user/user.hpp"
#include "evir/ir/user/function.hpp"

namespace evir
{

/// @brief A Class representing a single stand-alone IR module
///
/// Has the following metadata by default:
/// - @link Metadata::BuiltinPropertyID @endlink::`META_MODULE_NAME` 
/// - @link Metadata::BuiltinPropertyID @endlink::`META_MODULE_ENTRYPOINT` 
class Module
{
	friend class Metadata;

	String name;
	Vector<Metadata*> metadata;
	Vector<User*> users;

public:

	#pragma region

	/// @brief Constructs a new module
	Module();

	/// @copydoc Module::Module()
	/// @param module_name the name of the module
	Module(String module_name);

	/// @returns The name of the module if set properly,
	/// @returns otherwise an empty string.
	String get_name();

	#pragma endregion
	#pragma region Metadata manipulation

	/// @brief Checks if the module has metadata with the given path
	bool has_metadata(Metadata::Path path);

	/// @brief Checks if the module has metadata with the given type
	bool has_metadata(Metadata::BuiltinPropertyID type);

	/// @brief Adds metadata to the module
	/// @param mdata the metadata to add
	/// @warning if the module already has metadata with the same path the function will abort
	void add_metadata(Metadata* mdata);

	/// @brief Adds metadata to the module
	/// @param id the id of the builtin property to add
	/// @param value the value of the metadata
	/// @warning if the module already has metadata with the same path the function will abort
	void add_metadata(Metadata::BuiltinPropertyID id, MDValue* value);

	/// @brief Sets a metadata property with the given path
	/// @param path the path of the metadata property
	/// @param value the value to set the property to
	/// @warning if the module doesn't have metadata with the given path the function will abort
	void set_metadata(Metadata::Path path, MDValue* value);

	/// @brief Sets a metadata property with the built-in type
	/// @param type the type of the metadata property
	/// @param value the value to set the property to
	/// @warning if the module doesn't have metadata with the given type the function will abort
	void set_metadata(Metadata::BuiltinPropertyID type, MDValue* value);

	/// @brief Gets the requested metadata property
	/// @param path the path of the metadata property
	/// @return a pointer to the metadata, or null if it isn't found
	Metadata* get_metadata(Metadata::Path path);

	/// @brief Gets the requested metadata property
	/// @param type the built-in type of the metadata property
	/// @return a pointer to the metadata, or null if it isn't found
	Metadata* get_metadata(Metadata::BuiltinPropertyID type);

	#pragma endregion
	#pragma region User manipulation

	/// @returns The function if it exists, or a new function
	/// @returns if it doesn't. A nullptr is returned if a user
	/// @returns with the same name exists with a different type.
	Function* get_or_insert_function(FunctionType* type, String name);

	// /// @returns The global if it exists, or a new global
	// /// @returns if it doesn't. A nullptr is returned if a user
	// /// @returns with the same name exists with a different type.
	// Global* get_or_insert_global(Global* type, String name);

	#pragma endregion
	#pragma region IR generation

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