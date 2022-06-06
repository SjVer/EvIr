extern crate evir;
use evir::ir::{Module, FunctionType, IntegerType, VoidType, IRBuilder};
use std::{io::Write, fs::File};

fn main() {
	// create module and builder
	let mut module = Module::new("fib");
	let mut builder = IRBuilder::new();

	// declare function `void printint(int x)`
	let rettype = VoidType::new();
	let params = vec![IntegerType::new(true, 32)];
	let _printint = module.get_or_create_function("printint", FunctionType::new(rettype, params)).unwrap();

	// declare function `int fib(int x)`
	let rettype = builder.get_i32_type();
	let params = vec![builder.get_i32_type()];
	let fib = module.get_or_create_function("fib", FunctionType::new(rettype, params)).unwrap();
	// fib:
	//   int fib(int x) {
	//     if(x <= 2) return 1;
	//     return fib(x - 1) + fib(x - 2);
	//   }

	// write to file
	write!(File::create("fib.evir").unwrap(), "{}", module.generate_ir()).unwrap();
}