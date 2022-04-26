#include "ir/irbuilder.hpp"
#include <iostream>

using namespace evir;

int main()
{
	// create module
	Module* module = new Module("test");
	IRBuilder* builder = new IRBuilder();
	
	// set metadata
	#pragma region 
	module->add_metadata(Metadata::MD_MODULE_SOURCE_FILENAME, new MDStringValue("test.evi"));
	module->add_metadata(Metadata::MD_MODULE_SOURCE_DIRECTORY, new MDStringValue("/home/sjoerdv/test/"));
	module->add_metadata(Metadata::MD_MODULE_SOURCE_LANGUAGE, new MDStringValue("Evi"));

	module->add_metadata(Metadata::MD_MODULE_PRODUCER_NAME, new MDStringValue("evi (official)"));
	module->add_metadata(Metadata::MD_MODULE_PRODUCER_VERSION, new MDArrayValue({
		(MDIRValue*)new IntegerConst(1), (MDIRValue*)new IntegerConst(0), (MDIRValue*)new IntegerConst(0) }));
	module->add_metadata(Metadata::MD_MODULE_PRODUCER_TYPE, new MDOptionValue("compiler"));

	module->add_metadata(Metadata::MD_TARGET_TRIPLE, new MDStringValue("x86_64-pc-linux-gnu"));
	module->add_metadata(Metadata::MD_TARGET_CPU, new MDStringValue("generic"));
	module->add_metadata(Metadata::MD_TARGET_DATALAYOUT, new MDOptionValue("auto"));
	module->add_metadata(Metadata::MD_TARGET_OPTIMIZATION, new MDOptionValue("O3"));

	module->add_metadata(Metadata::MD_DEBUG_GENERATE, new MDIRValue(Constant::new_integer(1)));
	module->add_metadata(Metadata::MD_DEBUG_INCLUDESOURCE, new MDIRValue(Constant::new_integer(1)));
	module->add_metadata(Metadata::MD_DEBUG_SOURCELOCATION, new MDOptionValue("modulefile"));
	module->add_metadata(Metadata::MD_DEBUG_SOURCECHECKSUM, new MDHexValue(0xbba3));
	module->add_metadata(Metadata::MD_DEBUG_DWARFVERSION, new MDIRValue(Constant::new_integer(5)));

	module->add_metadata(Metadata::MD_DEBUG_TYPENAMES, new MDMapValue({
		{ new MDStringValue("bln"), new MDTypeValue(Type::get_integer_type(false, 1)) },
		{ new MDStringValue("chr"), new MDTypeValue(Type::get_integer_type(false, 8)) },
		{ new MDStringValue("flt"), new MDTypeValue(Type::get_float_type(32)) },
		{ new MDStringValue("i32"), new MDTypeValue(Type::get_integer_type(true, 32)) },
	}));
	#pragma endregion

	// add functions
	#pragma region

	// puts
	{
		FunctionType* functype = new FunctionType(new IntegerType(false, 8), {});
		Function* func = module->get_or_insert_function(functype, "puts");
		func->add_property("nonstatic");
	}

	// iseven
	{
		FunctionType* functype = new FunctionType(new IntegerType(false, 1), {new FloatType(32)});
		Function* func = module->get_or_insert_function(functype, "iseven");
		func->add_property("nonstatic");

		BasicBlock* bentry = new BasicBlock("entry");
		func->append_block(bentry);

		BasicBlock* b0 = new BasicBlock();
		b0->add_predecessor(bentry);
		func->append_block(b0);
	}

	// main
	{
		FunctionType* functype = new FunctionType(new IntegerType(true, 32), {
			new IntegerType(true, 32), (new IntegerType(false, 8))->get_pointer_to(2) });
		Function* func = module->get_or_insert_function(functype, "main");
		func->add_property("nonstatic");

		BasicBlock* bentry = new BasicBlock("entry");
		func->append_block(bentry);
		{
			builder->set_block(bentry);
			builder->create_ret(new FloatConst(0));
		}

		BasicBlock* b0 = new BasicBlock();
		b0->add_predecessor(bentry);
		func->append_block(b0);

		BasicBlock* b1 = new BasicBlock();
		b1->add_predecessor(bentry);
		b1->add_predecessor(b0);
		func->append_block(b1);

		module->add_metadata(Metadata::MD_MODULE_ENTRYPOINT, (MDIRValue*)(new Reference(func)));
	}

	#pragma endregion

	std::cout << module->generate_ir();

	return 0;
}