//===--- ir/instruction/mod.rs --------- Rust ---===
// This header is part of the EvIr library
// MIT License - Sjoerd Vermeulen (2022)
// For more info see https://github.com/SjVer/EvIr
//===--------------------------------------------===

use crate::{
	Ptr, UNRESOLVED,
	ir::{BasicBlock, IR, Value, generate_ir_comment}
};

#[derive(Debug, Clone)]
pub enum Instruction {
	Br(Ptr<BasicBlock>),
	CondBr(Value, Ptr<BasicBlock>, Option<Ptr<BasicBlock>>),
	Ret(Value),
	Disp(Value),

	#[doc(hidden)]
	__Cx01mnt(String),
}

// per-inst ir generation
impl Instruction {
	fn br_irgen(&self, dest: &Ptr<BasicBlock>) -> IR {
		let label = if dest.is_null() {
			UNRESOLVED.to_string()
		} else {
			dest.as_ref().get_ir_label()
		};

		format!(".br &{}", label)
	}

	fn condbr_irgen(&self, 
		condition: &Value,
		tdest: &Ptr<BasicBlock>,
		fdest: &Option<Ptr<BasicBlock>>
	) -> IR {
		let tlabel = if tdest.is_null() {
			UNRESOLVED.to_string()
		} else {
			tdest.as_ref().get_ir_label()
		};
		
		match fdest {
			Some(d) => {
				let flabel = if d.is_null() {
					UNRESOLVED.to_string()
				} else {
					d.as_ref().get_ir_label()
				};

				format!(".condbr {} &{} &{}", condition.generate_ir(), tlabel, flabel)
			}
			None => format!(".condbr {} &{}", condition.generate_ir(), tlabel)
		}
	}

	fn ret_irgen(&self, value: &Value) -> IR {
		format!(".ret {}", value.generate_ir())
	}

	fn disp_irgen(&self, value: &Value) -> IR {
		format!(".disp {}", value.generate_ir())
	}
}

impl Instruction {
	pub fn is_terminator(&self) -> bool {
		match self {
			Self::Ret(..) | Self::Br(..) |
			Self::CondBr(_, _, Some(_)) => true,
			_ => false,
		}
	}

	pub fn generate_ir(&self) -> IR {
		match self {
			Self::Br(d) => self.br_irgen(d),
			Self::CondBr(c, t, f) => self.condbr_irgen(c, t, f),
			Self::Ret(v) => self.ret_irgen(v),
			Self::Disp(v) => self.disp_irgen(v),
			Self::__Cx01mnt(s) => generate_ir_comment(s, false).trim_end().to_string(),
		}
	}
}