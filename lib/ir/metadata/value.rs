//===--- ir/metadata/value.rs ---------- Rust ---===
// This header is part of the EvIr library
// MIT License - Sjoerd Vermeulen (2022)
// For more info see https://github.com/SjVer/EvIr
//===--------------------------------------------===

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

pub(crate) trait ToMDValue {
	fn to_mdvalue(self) -> MDValue;
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