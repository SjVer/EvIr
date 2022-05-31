extern crate evir;
use evir::ir::{Module, FunctionType, IntegerType, VoidType};
use std::{io::Write, fs::File};

fn main() {
	// create module and builder
	let mut module = Module::new("fib");

	// declare function `void printint(int x)`
	let rettype = VoidType::new();
	let params = vec![IntegerType::new(true, 32)];
	let printint = module.get_or_create_function("printint", FunctionType::new(rettype, params)).unwrap();

	// write to file
	write!(File::create("fib.evir").unwrap(), "{}", module.generate_ir()).unwrap();
}