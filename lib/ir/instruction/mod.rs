//===--- ir/instruction/mod.rs --------- Rust ---===
// This header is part of the EvIr library
// MIT License - Sjoerd Vermeulen (2022)
// For more info see https://github.com/SjVer/EvIr
//===--------------------------------------------===

use super::{IR, Value, generate_ir_comment};

#[derive(Debug, Clone)]
pub enum Instruction {
	Disp(Value),
	__Cx01mnt(String),
}

// per-inst ir generation
impl Instruction {
	fn disp_irgen(&self, value: &Value) -> IR {
		format!(".disp {}", value.generate_ir())
	}
}

impl Instruction {
	pub fn is_terminator(&self) -> bool {
		match self {
			_ => false,
		}
	}

	pub fn generate_ir(&self) -> IR {
		match self {
			Self::Disp(v) => self.disp_irgen(v),
			Self::__Cx01mnt(s) => generate_ir_comment(s, false),
		}
	}
}