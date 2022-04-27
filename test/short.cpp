#include <evir/ir/irbuilder.hpp>
#include <fstream>

using namespace evir;

int main()
{
	Module* mod = new Module("test");
	IRBuilder* builder = new IRBuilder();

	// create main function
	FunctionType* ftype = new FunctionType((Type*)builder->get_int32_type(), {});
	Function* func = mod->get_or_insert_function(ftype, "main");

	// create basic block
	BasicBlock* block = new BasicBlock("entry");
	func->append_block(block);
	builder->set_block(block);

	// build instructions
	Constant* strconst = Constant::new_string("Hello World!\n");
	builder->create_disp(strconst);
	builder->create_ret(Constant::new_integer(0));

	// set entrypoint
	mod->add_metadata(Metadata::MD_MODULE_ENTRYPOINT, (MDIRValue*)new Reference(func));

	// disable debbuging information
	mod->add_metadata(Metadata::MD_DEBUG_GENERATE, new MDOptionValue("false"));

	// generate ir
	std::ofstream s("test.evir");
	s << mod->generate_ir();
	s.close();
}