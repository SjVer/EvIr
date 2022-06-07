extern crate evir;
use evir::ir::{Module, FunctionType, IntegerType, VoidType, IRBuilder, BasicBlock};
use std::{io::Write, fs::File};

fn main() {
	// create module and builder
	let mut module = Module::new("fib");
	let mut builder = IRBuilder::new();

	// tmp val
	let todo = builder.get_false();


	// declare function `void printint(int x)`
	let _printint = {
		let rettype = VoidType::new();
		let params = vec![IntegerType::new(true, 32)];
		module.get_or_create_function("printint", FunctionType::new(rettype, params)).unwrap()
	};


	// declare function `int fib(int x)`
	let fib = {
		let rettype = builder.get_i32_type();
		let params = vec![builder.get_i32_type()];
		module.get_or_create_function("fib", FunctionType::new(rettype, params)).unwrap()
	};
	let fentry = fib.append_block(BasicBlock::new_labeled("entry"));
	let fret_1 = fib.append_block(BasicBlock::new_labeled("ret_1"));
	
	// if(x <= 2) return 1;
	builder.set_insert_block(fentry);
	builder.append_comment("if(x <= 2) return 1;\nreturn fib(x - 1) + fib(x - 2)");
	let cond = builder.build_cmple(&todo, builder.get_int(2));
	builder.build_condbr(cond, fret_1.as_ref(), None);
	
	// return fib(x - 1) + fib(x - 2);
	let retval = builder.build_add(
		builder.build_call(fib, vec![builder.build_sub(&todo, builder.get_int(1))]),
		builder.build_call(fib, vec![builder.build_sub(&todo, builder.get_int(2))]),
	);
	builder.build_ret(retval);
	builder.set_insert_block(fret_1);
	builder.build_ret(builder.get_int(1));


	// write to file
	write!(File::create("fib.evir").unwrap(), "{}", module.generate_ir()).unwrap();
}