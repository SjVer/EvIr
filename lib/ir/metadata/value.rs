//===--- ir/metadata/value.rs ---------- Rust ---===
// This header is part of the EvIr library
// MIT License - Sjoerd Vermeulen (2022)
// For more info see https://github.com/SjVer/EvIr
//===--------------------------------------------===

use crate::{TAB, ir::IR};
use std::collections::HashMap;

pub enum MDValue {
	Hex(u128),
	String(String),
	Array(Vec<MDValue>),
	Map(HashMap<MDValue, MDValue>),
	// Option(?),
	// Type(Type),
	// IRValue(Value).
}

impl MDValue {
	pub fn generate_ir(&self) -> IR {
		match self {
			Self::Hex(v) => format!("{:#x}", v),
			Self::String(s) => format!("{:?}", s), // s.escape_default().to_string(),
			Self::Array(a) => {
				let items: Vec<String> = a.iter().map(|v| v.generate_ir()).collect();
				format!("[{}]", items.join(", "))
			}
			Self::Map(m) => {
				let items: Vec<String> = m.iter().map(
					|(k, v)| format!("{}{}: {}", TAB, k.generate_ir(), v.generate_ir())
				).collect();
				format!("{{ {} }}", items.join(",\n"))
			}
		}
	}
}

// conversion to mdvalue

pub trait ToMDValue {
	fn to_mdvalue(self) -> MDValue;
}

impl ToMDValue for MDValue {
	fn to_mdvalue(self) -> MDValue {
		self
	}
}

impl ToMDValue for u128 {
	fn to_mdvalue(self) -> MDValue {
		MDValue::Hex(self)
	}
}

impl ToMDValue for String {
	fn to_mdvalue(self) -> MDValue {
		MDValue::String(self)
	}
}

impl ToMDValue for Vec<MDValue> {
	fn to_mdvalue(self) -> MDValue {
		MDValue::Array(self)
	}
}

impl ToMDValue for HashMap<MDValue, MDValue> {
	fn to_mdvalue(self) -> MDValue {
		MDValue::Map(self)
	}
}

// TODO: impl's for Option, Type & IRValue