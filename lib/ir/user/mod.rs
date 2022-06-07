//===--- ir/user/mod.rs ---------------- Rust ---===
// This header is part of the EvIr library
// MIT License - Sjoerd Vermeulen (2022)
// For more info see https://github.com/SjVer/EvIr
//===--------------------------------------------===

mod function;
mod local;

pub use function::*;
pub use local::*;

use super::{IR, generate_ir_comment};

pub trait User {
	fn get_properties(&mut self) -> &mut Vec<String>;
	fn is_defined(&self) -> bool;

	fn generate_properties_comment(&mut self) -> IR {
		let mut props = self.get_properties().clone();

		props.push(String::from(if self.is_defined() { "defined" } else { "declared" }));
		
		if !props.is_empty() {
			generate_ir_comment(format!("props: {}", props.join(", ")), false)
		} else { IR::new() }
	}
}