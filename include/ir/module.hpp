//===---------- ir/module.hpp --- C++ -----------===
// This header is part of the EviIr library 		
// MIT License - Sjoerd Vermeulen (2022)			
// For more info see https://github.com/SjVer/EviIr	
//===--------------------------------------------===

#ifndef EVIIR_IR_MODULE_H
#define EVIIR_IR_MODULE_H

#include ".common.hpp"

namespace eviir
{

class Module
{
	string name;

public:
	Module(string module_name);

	string to_string();
};

}

#endif // EVIIR_IR_MODULE_H