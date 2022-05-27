#include <evir/ir/irbuilder.hpp>
#include <fstream>

using namespace evir;

int main()
{
	// create module
	Module* module = new Module("fib");
	IRBuilder* builder = new IRBuilder();
	
	Value* ZERO = Constant::new_integer(0);

	// printint
	Function* printint = module->get_or_insert_function(
		new FunctionType(builder->get_void_type(), {}),
		"printint"
	);


	// fib:
	//   int fib(int x) {
	//     if(x <= 2) return 1;
	//     return fib(x - 1) + fib(x - 2);
	//   }
	Function* fib = module->get_or_insert_function(
		new FunctionType(builder->get_int32_type(), {builder->get_int32_type()}),
		"fib"
	);
	BasicBlock* fentry = new BasicBlock("entry");
	BasicBlock* fret1 = new BasicBlock("ret_1");
	fib->append_block(fentry);
	fib->append_block(fret1);

	builder->set_block(fentry);
	Value* cond = builder->create_cmplteq(ZERO, Constant::new_integer(2)); // x <= 2
	builder->create_condbr(cond, fret1);
	
	Value* retval = builder->create_add(
		builder->create_call(fib, {builder->create_sub(ZERO, Constant::new_integer(1))}),
		builder->create_call(fib, {builder->create_sub(ZERO, Constant::new_integer(2))})
	); // fib(x - 1) + fib(x - 2)
	builder->create_ret(retval);
	builder->set_block(fret1);
	builder->create_ret(Constant::new_integer(1));


	// main
	Function* mainf = module->get_or_insert_function(
		new FunctionType(builder->get_int32_type(), {}),
		"main"
	);
	BasicBlock* mentry = new BasicBlock("entry");
	mainf->append_block(mentry);
	builder->set_block(mentry);
	// builder->create_disp(builder->create_call(fib, {Constant::new_integer(5)}));

	
	module->add_metadata(Metadata::MD_MODULE_ENTRYPOINT, (MDIRValue*)(new Reference(mainf)));

	std::ofstream s("test/fib.evir");
	s << module->generate_ir();
	s.close();

	return 0;
}