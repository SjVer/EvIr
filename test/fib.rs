extern crate evir;
use evir::{
	ir::{
		Module, FunctionType,
		IRBuilder, BasicBlock,
		ToRef, BuiltinMDProp::ModuleEntrypoint, Metadata},
	passes::validate::Validator,
};
use std::{io::Write, fs::File};

fn main() {
	// create module and builder
	let mut module = Module::new("fib");
	let mut builder = IRBuilder::new();


	// declare function `int fib(int x)`
	let fib = {
		let rettype = builder.get_i32_type();
		let params = vec![builder.get_i32_type()];
		module.get_or_create_function("fib", FunctionType::new(rettype, params)).unwrap()
	};
	let fentry = fib.append_block(BasicBlock::new_labeled("entry"));
	let fret_1 = fib.append_block(BasicBlock::new_labeled("ret_1"));
	let x = fib.get_param(0).to_ref();
	
	// if(x <= 2) return 1;
	builder.set_insert_block(fentry);
	builder.append_comment("if(x <= 2) return 1;\nreturn fib(x - 1) + fib(x - 2);");
	let cond = builder.build_cmple(x.clone(), builder.get_int(2));
	builder.build_condbr(cond, fret_1.as_ref(), None);

	// return fib(x - 1) + fib(x - 2);
	let retval = builder.build_add(
		builder.build_call(fib, vec![builder.build_sub(x.clone(), builder.get_int(1))]),
		builder.build_call(fib, vec![builder.build_sub(x.clone(), builder.get_int(2))]),
	);
	builder.build_ret(retval);
	builder.set_insert_block(fret_1);
	builder.build_ret(builder.get_int(1));


	// set entry point
	module.set_metadata(Metadata::new(ModuleEntrypoint, fib.to_ref()))

	// validate and write to file
	if let Err(d) = Validator::new().validate_module(&module) {
		d.dispatch();
	} else {
		write!(File::create("fib.evir").unwrap(), "{}", module.generate_ir()).unwrap();
	}
}