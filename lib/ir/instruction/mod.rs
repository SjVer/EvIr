//===--- ir/instruction/mod.rs --------- Rust ---===
// This header is part of the EvIr library
// MIT License - Sjoerd Vermeulen (2022)
// For more info see https://github.com/SjVer/EvIr
//===--------------------------------------------===

use super::IR;

#[derive(Debug, Clone)]
pub enum Instruction {
	
}

impl Instruction {
	pub fn is_terminator(&self) -> bool {
		// TODO: this
		false
	}

	pub fn generate_ir(&self) -> IR {
		IR::new()
	}
}