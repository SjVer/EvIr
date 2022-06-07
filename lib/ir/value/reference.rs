//===--- ir/value/reference.rs --------- Rust ---===
// This header is part of the EvIr library
// MIT License - Sjoerd Vermeulen (2022)
// For more info see https://github.com/SjVer/EvIr
//===--------------------------------------------===

use crate::{
	Ptr,
	ir::{
		IR, Value, ToValue,
		Function, Local, Param,
	}, UNRESOLVED,
};

#[derive(Debug, Clone)]
pub enum Reference {
	Function(Ptr<Function>),
	Local(Ptr<Local>),
	Param(Ptr<Param>),
}

impl Reference {
	pub fn generate_ir(&self) -> IR {
		macro_rules! get_name {
			($var:expr) => {
				if $var.is_null() { UNRESOLVED.to_string() } else { $var.as_ref().get_ir_name() } 
			};
		}

		let name = match self {
			Self::Function(p) => get_name!(p),
			Self::Local(p) => get_name!(p),
			Self::Param(p) => get_name!(p),
		};

		format!("%{}", name)
	}
}

impl ToValue for Reference {
	fn to_value(self) -> super::Value {
		Value::Reference(self)
	}
}


pub trait ToRef {
	fn to_ref(&self) -> Reference;
}

impl ToRef for &Function {
	fn to_ref(&self) -> Reference {
		Reference::Function(Ptr::new(self))
	}
}

impl ToRef for &Local {
	fn to_ref(&self) -> Reference {
		Reference::Local(Ptr::new(self))
	}
}

impl ToRef for &Param {
	fn to_ref(&self) -> Reference {
		Reference::Param(Ptr::new(self))
	}
}