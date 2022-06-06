//===--- ir/value/mod.rs --------------- Rust ---===
// This header is part of the EvIr library
// MIT License - Sjoerd Vermeulen (2022)
// For more info see https://github.com/SjVer/EvIr
//===--------------------------------------------===

mod constant;
mod operator;

pub use constant::*;
pub use operator::*;

use super::IR;

#[derive(Debug, Clone)]
pub enum Value {
	Constant(Constant),
	Operator(Operator),
	Reference(),
}

// constructors 
impl Value {
	pub fn operator() -> Self {
		
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
			Self::Reference() => IR::new(),
		}
	}
}