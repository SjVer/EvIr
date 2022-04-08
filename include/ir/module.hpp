//===---------- ir/module.hpp --- C++ -----------===
// This header is part of the EviIr library 		
// MIT License - Sjoerd Vermeulen (2022)			
// For more info see https://github.com/SjVer/EviIr	
//===--------------------------------------------===

#ifndef EVIIR_IR_MODULE_H
#define EVIIR_IR_MODULE_H

#include ".common.hpp"
#include "ir/metadata.hpp"

namespace eviir
{

class Module
{
	string name;
	// Metadata* metadata;

	// TODO:
	//	- generate_comment function
	//  - metadata shit
	//  - everything else lmao

public:

	/// Constructs a new module
	/// @param module_name the name of the module
	Module(string module_name);

	/// Generates the IR of the current module's
	/// metadata and returns it as a string
	string generate_metadata_ir();

	/// Generates the IR of the current module and 
	/// returns it as a string
	string generate_ir();
};

}

#endif // EVIIR_IR_MODULE_H