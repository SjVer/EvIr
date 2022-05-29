//===--- ir/module.rs ------------------ Rust ---===
// This header is part of the EvIr library
// MIT License - Sjoerd Vermeulen (2022)
// For more info see https://github.com/SjVer/EvIr
//===--------------------------------------------===

use crate::{
	evir_assert,
	ir::metadata::{Metadata, MDValue, BuiltinMDProp, ToMDPath}
};

/// A single stand-alone IR module. \
/// Has the following metadata by default:
/// - [!module/name][`ir::metadata::BuiltinMDProp::Module_Name`]
/// - [!debug/generate][`ir::metadata::BuiltinMDProp::Debug_Generate`]
#[derive(Default)]
pub struct Module {
	metadata: Vec<Metadata>,
	// users: Vec<User>,	
}

// misc stuff
impl Module {
	pub fn new(name: String) -> Self {
		let mut this = Self::default();
		this.add_metadata(Metadata::new(BuiltinMDProp::ModuleName, name));
		this.add_metadata(Metadata::new(BuiltinMDProp::DebugGenerate, 0));

		this
	}

	pub fn get_name(&self) -> Option<String> {
		match self.get_metadata(BuiltinMDProp::ModuleName) {
			Some(md) => match &md.value {
				MDValue::String(s) => Some(s.clone()),
				_ => None,
			}
			None => None,
		}
	}
}

// metadata stuff
impl Module {
	pub fn get_metadata(&self, path: impl ToMDPath) -> Option<&Metadata> {
		let path = path.to_mdpath();

		for md in &self.metadata {
			if md.path == path { return Some(md); }
		}

		None
	}

	pub fn has_metadata(&self, path: impl ToMDPath) -> bool {
		self.get_metadata(path).is_some()
	}

	pub fn add_metadata(&mut self, md: Metadata) {
		evir_assert!(!self.has_metadata(md.path.clone()), "Module already has metadata with same path");
		self.metadata.push(md);
	}
}