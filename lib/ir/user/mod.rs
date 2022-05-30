//===--- ir/user/mod.rs ---------------- Rust ---===
// This header is part of the EvIr library
// MIT License - Sjoerd Vermeulen (2022)
// For more info see https://github.com/SjVer/EvIr
//===--------------------------------------------===

mod function;

pub use function::*;

use super::{IR, generate_ir_comment, Module};

pub struct User<'a> {
	name: Option<String>,
	properties: Vec<String>,
	user: UserE,
	parent: &'a mut Module<'a>,
}

impl<'a> User<'a> {
	fn get_name(&self) -> Option<String> {
		self.name.clone()
	}

	fn get_parent(&'a self) -> &'a mut Module {
		self.parent
	}

	fn add_property(&mut self, property: impl ToString) {
		self.properties.push(property.to_string());
	}

	fn generate_properties_comment(&self) -> IR {
		let mut props = self.properties.clone();
		props.push(String::from(if self.user.is_defined() { "defined" } else { "declared" }));
		
		if !props.is_empty() {
			generate_ir_comment(format!("props: {}", props.join(", ")), false)
		} else { IR::new() }
	}
}

enum UserE {
	Function(Function),
	// Global(Global),
}

pub trait UserT {
	fn is_defined(&self) -> bool;
	fn generate_ir(&self) -> IR;
}