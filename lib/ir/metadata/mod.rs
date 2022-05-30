//===--- ir/metadata/mod.rs ------------ Rust ---===
// This header is part of the EvIr library
// MIT License - Sjoerd Vermeulen (2022)
// For more info see https://github.com/SjVer/EvIr
//===--------------------------------------------===

mod value;
mod path;

pub use value::*;
pub use path::*;

use super::IR;

pub struct Metadata {
	pub(crate) path: MDPath,
	pub(crate) value: MDValue,
}

impl Metadata {
	pub fn new(path: impl ToMDPath, value: impl ToMDValue) -> Self {
		Self {
			path: path.to_mdpath(),
			value: value.to_mdvalue(),
		}
	}

	pub fn generate_ir(&self) -> IR {
		format!("!{} {}\n", self.path.to_string(), self.value.generate_ir())
	}
}