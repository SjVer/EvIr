//===--- passes/validate.rs ------------ Rust ---===
// This header is part of the EvIr library
// MIT License - Sjoerd Vermeulen (2022)
// For more info see https://github.com/SjVer/EvIr
//===--------------------------------------------===

use crate::{
	frontend::Diagnostic,
	ir::{Module, Metadata, BuiltinMDProp, __Tomdpath},
};

pub type ValidateResult = Result<(), Diagnostic>;

/// An "interface" used to validate IR
pub struct Validator {

}

macro_rules! check {
	($cond:expr => $($fmt:tt)*) => {
		if !($cond) { return Err(Diagnostic::new(format!($($fmt)*))); }
	};
}

// public stuff
impl Validator {
	pub fn new() -> Self {
		Self {

		}
	}
	fn reset(&mut self) { *self = Validator::new(); }

	pub fn validate_module(&mut self, module: &Module) -> ValidateResult {
		self.reset();

		self.validate_metadata(&module.metadata)?;

		Ok(())
	}

	pub fn validate_metadata(&mut self, metadata: &Vec<Metadata>) -> ValidateResult {
		self.reset();

		// required metadata
		for path in vec![
			BuiltinMDProp::ModuleName,
			BuiltinMDProp::ModuleEntrypoint,
			BuiltinMDProp::DebugGenerate,
		] {
			let path = path.to_mdpath();
			check!(metadata.iter().find(|md| md.path == path).is_some() => "Missing metadata `!{}`", path.to_string());
		}

		Ok(())
	}
}