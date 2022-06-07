//===--- ir/value/mod.rs --------------- Rust ---===
// This header is part of the EvIr library
// MIT License - Sjoerd Vermeulen (2022)
// For more info see https://github.com/SjVer/EvIr
//===--------------------------------------------===

mod constant;
mod operator;
mod reference;

use std::fmt::Debug;

pub use constant::*;
pub use operator::*;
pub use reference::*;

use crate::ir::IR;

#[derive(Debug, Clone)]
pub enum Value {
	Constant(Constant),
	Operator(Operator),
	Reference(Reference),
}

// constructors 
impl Value {
	pub fn operator(op: impl Op) -> Self {
		Self::Operator(op.to_operator())
	}
}

impl Value {
	pub fn is_constant(&self) -> bool {
		if let Self::Operator(op) = self {
			op.is_constant()
		} else {
			matches!(self, Self::Constant(..))
		}
	}

	pub fn generate_ir(&self) -> IR {
		match self {
			Self::Constant(c) => c.generate_ir(),
			Self::Operator(op) => op.generate_ir(),
			Self::Reference(r) => r.generate_ir(),
		}
	}
}


impl ToValue for Value {
	fn to_value(self) -> Value {
		self
	}
}
impl ToValue for &Value {
	fn to_value(self) -> Value {
		self.clone()
	}
}

pub trait ToValue {
	fn to_value(self) -> Value;
}