//===---------- ir/module.hpp --- C++ -----------===
// This header is part of the EviIr library 		
// MIT License - Sjoerd Vermeulen (2022)			
// For more info see https://github.com/SjVer/EviIr	
//===--------------------------------------------===

#ifndef EVIIR_IR_MODULE_H
#define EVIIR_IR_MODULE_H

#include ".common.hpp"
#include "ir/metadata.hpp"

#define __IR_COMMENT_LENGTH 49
#define __IR_HCOMMENT_LENGTH 50
#define __IR_HCOMMENT_SURROUND_CHAR '='
#define __IR_HCOMMENT_MIN_SURROUND 3

namespace eviir
{

class Module
{
	friend class Metadata;

	/// @section Private members

	string name;
	vector<Metadata*> metadata;

public:

	/// @section Constructors

	/// Constructs a new module
	/// @param module_name the name of the module
	Module(string module_name);

	/// @section Manipulation

	/// Checks if the module has metadata with the given path
	bool has_metadata(vector<string> path);

	/// Adds metadata to the module
	/// @param mdata the metadata to add
	/// @warning aborts if the module already has metadata with the same path
	void add_metadata(Metadata* mdata);

	/// @section IR generation

	/// Generates an IR comment
	/// @param text the text of the comment
	/// @param header if true, the comment is formatted as a header comment
	/// @return the IR as a string (with newline)
	string generate_ir_comment(string text, bool header = false);

	/// Generates the IR of the current module's metadata
	/// @return the IR as a string (with newline)
	string generate_metadata_ir();

	/// Generates the IR of the current module
	/// @return the IR as a string (with newline)
	string generate_ir();
};

}

#endif // EVIIR_IR_MODULE_H