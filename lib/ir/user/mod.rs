//===--- ir/user/mod.rs ---------------- Rust ---===
// This header is part of the EvIr library
// MIT License - Sjoerd Vermeulen (2022)
// For more info see https://github.com/SjVer/EvIr
//===--------------------------------------------===

mod function;

pub use function::*;

use super::{IR, generate_ir_comment, Module};

/*
fn generate_properties_comment(&self) -> IR {
	let mut props = self.properties.clone();
	props.push(String::from(if self.user.is_defined() { "defined" } else { "declared" }));
	
	if !props.is_empty() {
		generate_ir_comment(format!("props: {}", props.join(", ")), false)
	} else { IR::new() }
}
*/