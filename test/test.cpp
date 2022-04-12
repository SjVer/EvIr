#include "ir/irbuilder.hpp"
#include <iostream>

using namespace evir;

int main()
{
	// create module
	Module* module = new Module("test");
	IRBuilder* builder = new IRBuilder();
	
	// set metadata
	module->add_metadata(new Metadata(Metadata::META_MODULE_SOURCE_FILENAME, Value::new_string("test.evi")));
	module->add_metadata(new Metadata(Metadata::META_MODULE_SOURCE_DIRECTORY, Value::new_string("/home/sjoerdv/test/")));
	module->add_metadata(new Metadata(Metadata::META_MODULE_SOURCE_LANGUAGE, Value::new_string("Evi")));

	module->add_metadata(new Metadata(Metadata::META_MODULE_PRODUCER_NAME, Value::new_string("evi (official)")));
	module->add_metadata(new Metadata(Metadata::META_MODULE_PRODUCER_VERSION, Value::new_list({
		Value::new_integer(1), Value::new_integer(0), Value::new_integer(0)	})));
	module->add_metadata(new Metadata(Metadata::META_MODULE_PRODUCER_TYPE, Value::new_option("compiler")));

	module->add_metadata(new Metadata(Metadata::META_TARGET_TRIPLE, Value::new_string("x86_64-pc-linux-gnu")));
	module->add_metadata(new Metadata(Metadata::META_TARGET_CPU, Value::new_string("generic")));
	module->add_metadata(new Metadata(Metadata::META_TARGET_DATALAYOUT, Value::new_option("auto")));
	module->add_metadata(new Metadata(Metadata::META_TARGET_OPTIMIZATION, Value::new_option("O3")));

	module->add_metadata(new Metadata(Metadata::META_DEBUG_GENERATE, Value::new_integer(1)));
	module->add_metadata(new Metadata(Metadata::META_DEBUG_INCLUDESOURCE, Value::new_integer(1)));
	module->add_metadata(new Metadata(Metadata::META_DEBUG_SOURCELOCATION, Value::new_option("modulefile")));
	module->add_metadata(new Metadata(Metadata::META_DEBUG_SOURCECHECKSUM, Value::new_integer(0x2fa0d9)));
	module->add_metadata(new Metadata(Metadata::META_DEBUG_DWARFVERSION, Value::new_integer(5)));

	module->add_metadata(new Metadata(Metadata::META_DEBUG_TYPENAMES, Value::new_map(Map<Value*, Value*>{
		{Value::new_string("bln"), Value::new_string("i.u.1")},
		{Value::new_string("chr"), Value::new_string("i.u.8")},
		{Value::new_string("flt"), Value::new_string("f.1.32")},
		{Value::new_string("i32"), Value::new_string("i.s.32")},
	})));


	std::cout << module->generate_ir();

	return 0;
}