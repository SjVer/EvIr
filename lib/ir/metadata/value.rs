//===--- ir/metadata/value.rs ---------- Rust ---===
// This header is part of the EvIr library
// MIT License - Sjoerd Vermeulen (2022)
// For more info see https://github.com/SjVer/EvIr
//===--------------------------------------------===

use crate::{TAB, ir::{IR, Value, Constant}, evir_assert};
use std::collections::HashMap;

#[derive(Clone)]
pub enum MDValue {
	Hex(u128),
	Array(Vec<MDValue>),
	Map(HashMap<MDValue, MDValue>),
	// Option(?),
	// Type(Type),
	IRValue(Value),
}

impl MDValue {
	pub fn generate_ir(&self) -> IR {
		match self {
			Self::Hex(v) => format!("{:#x}", v),
			Self::Array(a) => {
				let items: Vec<String> = a.iter().map(|v| v.generate_ir()).collect();
				format!("[{}]", items.join(", "))
			}
			Self::Map(m) => {
				let items: Vec<String> = m.iter().map(
					|(k, v)| format!("{}{}: {}", TAB, k.generate_ir(), v.generate_ir())
				).collect();
				format!("{{ {} }}", items.join(",\n"))
			},
			Self::IRValue(v) => v.generate_ir(),
		}
	}
}

// conversion to mdvalue

#[doc(hidden)]
pub trait __Tomdvalue {
	fn to_mdvalue(self) -> MDValue;
}

impl __Tomdvalue for MDValue {
	fn to_mdvalue(self) -> MDValue {
		self
	}
}

impl __Tomdvalue for u128 {
	fn to_mdvalue(self) -> MDValue {
		MDValue::Hex(self)
	}
}

impl __Tomdvalue for Vec<MDValue> {
	fn to_mdvalue(self) -> MDValue {
		MDValue::Array(self)
	}
}

impl __Tomdvalue for HashMap<MDValue, MDValue> {
	fn to_mdvalue(self) -> MDValue {
		MDValue::Map(self)
	}
}

// TODO: impl's for Option & Type

impl __Tomdvalue for Value {
	fn to_mdvalue(self) -> MDValue {
		evir_assert!(self.is_constant(), "cannot convert non-constant ir-value to md-value.");
		MDValue::IRValue(self)
	}
}

impl __Tomdvalue for Constant {
	fn to_mdvalue(self) -> MDValue {
		MDValue::IRValue(Value::Constant(self))
	}
}