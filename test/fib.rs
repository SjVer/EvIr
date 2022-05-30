extern crate evir;
use evir::ir::{
	Module
};
use std::{io::Write, fs::File};

fn main() {
	let mut module = Module::new("fib");


	write!(File::create("fib.evir").unwrap(), "{}", module.generate_ir()).unwrap();
}