//===--- ir/value/constant.rs ---------- Rust ---===
// This header is part of the EvIr library
// MIT License - Sjoerd Vermeulen (2022)
// For more info see https://github.com/SjVer/EvIr
//===--------------------------------------------===

// use std::collections::HashMap;
use crate::ir::{IR, Value, ToValue}; //, ENDL, TAB};

#[derive(Debug, Clone)]
pub enum Constant {
	Integer(u64),
	Float(f64),
	Array(Vec<Self>),
	// Map(HashMap<Self, Self>),

	Character(u8),
	String(String),
}

impl Constant {
	pub fn generate_ir(&self) -> IR {
		match self {
			Self::Integer(i) => i.to_string(),
			Self::Float(f) =>  if f != &0.0 { f.to_string() } else { IR::from("0.0") },
			Self::Array(v) => {
				let irs: Vec<IR> = v.iter().map(|v| v.generate_ir()).collect();
				format!("[{}]", irs.join(", "))
			}
			// Self::Map(m) => {
			// 	let items: Vec<IR> = m.iter().map(
			// 		|(k, v)| format!("{}: {}", k.generate_ir(), v.generate_ir())
			// 	).collect();
			//
			// 	if m.len() <= 1 {
			// 		format!("{{ {} }}", items.join(", "))
			// 	} else {
			// 		format!("{{ {} }}", items.join(&format!(",{}{}", ENDL, TAB)))
			// 	}
			// }
			
			Self::Character(c) => format!("'{}'", IR::from_utf8(vec![c.clone()]).unwrap()),
			Self::String(s) => format!("\"{}\"", s.escape_default()),
		}
	}
}

impl ToValue for Constant {
	fn to_value(self) -> super::Value {
		Value::Constant(self)
	}
}