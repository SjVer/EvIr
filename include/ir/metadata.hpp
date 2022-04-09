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

class Module;

class Metadata
{
public:

	/// The built-in metadata types
	typedef enum
	{
		META_MODULE_NAME,				// module/name
		META_MODULE_ENTRYPOINT,			// module/entrypoint

		META_MODULE_SOURCE_FILENAME,	// source/filename
		META_MODULE_SOURCE_DIRECTORY,	// source/directory
		META_MODULE_SOURCE_LANGUAGE,	// source/language

		META_MODULE_PRODUCER_NAME,		// producer/name
		META_MODULE_PRODUCER_VERSION,	// producer/version
		META_MODULE_PRODUCER_TYPE,		// producer/type

		META_TARGET_TRIPLE,				// target/triple
		META_TARGET_CPU,				// target/cpu
		META_TARGET_DATALAYOUT,			// target/datalayout
		META_TARGET_OPTIMIZATION,		// target/optimization

		META_DEBUG_GENERATE,			// debug/generate
		META_DEBUG_INCLUDESOURCE,		// debug/includesource
		META_DEBUG_SOURCELOCATION,		// debug/sourcelocation
		META_DEBUG_SOURCECHECKSUM,		// debug/sourcechecksum
		META_DEBUG_DWARFVERSION,		// debug/dwarfversion
		META_DEBUG_TYPENAMES,			// debug/typenames

		META_CUSTOM,
		META_NONE,
	} BuiltinProperty;

private:

	BuiltinProperty property_type;
	vector<string> property_path;
	Value* property_value;

public:

	/// Constructs a new metadata instance
	/// @param property the built-in property that this metadata defines
	Metadata(BuiltinProperty property);

	/// Generates the IR for this metadata property
	/// @param module the module owning this metadata (optional)
	/// @return the ir as a string (including a newline)
	string generate_ir(Module* module = nullptr);

};

};

#endif // EVIIR_IR_METADATA_H