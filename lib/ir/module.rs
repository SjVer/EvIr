//===--- ir/module.rs ------------------ Rust ---===
// This header is part of the EvIr library
// MIT License - Sjoerd Vermeulen (2022)
// For more info see https://github.com/SjVer/EvIr
//===--------------------------------------------===

use crate::{
	ENDL,
	ir::{
		IR,
		generate_ir_comment,
		metadata::*
	}
};

/// A single stand-alone IR module. \
/// Has the following metadata by default:
/// - [!module/name][`BuiltinMDProp::ModuleName`]
/// - [!debug/generate][`BuiltinMDProp::DebugGenerate`]
pub struct Module {
	metadata: Vec<Metadata>,
	// users: Vec<User>,	
}

// misc stuff
impl Module {
	pub fn new(name: impl ToString) -> Self {
		let mut this = Self {
			metadata: vec![]
		};

		this.set_metadata(Metadata::new(BuiltinMDProp::ModuleName, name.to_string()));
		this.set_metadata(Metadata::new(BuiltinMDProp::DebugGenerate, 0));

		this
	}

	pub fn get_name(&mut self) -> Option<String> {
		match self.get_metadata(BuiltinMDProp::ModuleName) {
			Some(v) => match &v {
				MDValue::String(s) => Some(s.clone()),
				_ => None,
			}
			None => None,
		}
	}
}

// metadata stuff
impl Module {
	/// Attempts to get (a mutable reference to) the 
	/// metadata value at the given [path][`MDPath`].
	pub fn get_metadata(&mut self, path: impl ToMDPath) -> Option<&mut MDValue> {
		let path = path.to_mdpath();

		for md in &mut self.metadata {
			if md.path == path { return Some(&mut md.value); }
		}

		None
	}

	/// Checks if the module has metadata set at
	/// the given path.
	pub fn has_metadata(&mut self, path: impl ToMDPath) -> bool {
		self.get_metadata(path).is_some()
	}

	/// Sets a metadata value at the given path.
	/// If a value was already set it is overwritten.
	pub fn set_metadata(&mut self, md: Metadata) {
		match self.get_metadata(md.path.clone()) {
			Some(old) => *old = md.value,
			None => self.metadata.push(md)
		}
	}
}

// ir generation
impl Module {
	fn generate_metadata_ir(&mut self, before_contents: bool) -> IR {
		// ===== first gather ir for each part seperatly =====
		
		let mut m  = IR::new(); // !module/...
		let mut ms = IR::new(); // !module/source/...
		let mut mp = IR::new(); // !module/producer/...
		let mut t  = IR::new(); // !target/...
		let mut d  = IR::new(); // !debug/...
		// let mut c  = IR::new(); // !...

		macro_rules! do_prop {
			($what:ident $path:path) => {
				if let Some(md) = self.get_metadata($path) {
					$what += &md.generate_ir();
				}
			};
		}

		do_prop!(m BuiltinMDProp::ModuleName);
		do_prop!(m BuiltinMDProp::ModuleEntrypoint);
		do_prop!(ms BuiltinMDProp::ModuleSourceFilename);
		do_prop!(ms BuiltinMDProp::ModuleSourceDirectory);
		do_prop!(ms BuiltinMDProp::ModuleSourceLanguage);
		// TODO: !module/source/<custom>
		do_prop!(mp BuiltinMDProp::ModuleProducerName);
		do_prop!(mp BuiltinMDProp::ModuleProducerVersion);
		do_prop!(mp BuiltinMDProp::ModuleProducerType);
		// TODO: !module/producer/<custom>
		// TODO: !module/<custom>
		do_prop!(t BuiltinMDProp::TargetTriple);
		do_prop!(t BuiltinMDProp::TargetCPU);
		do_prop!(t BuiltinMDProp::TargetDatalayout);
		do_prop!(t BuiltinMDProp::TargetOptimization);
		// TODO: !target/<custom>
		do_prop!(d BuiltinMDProp::DebugGenerate);
		do_prop!(d BuiltinMDProp::DebugIncludesource);
		do_prop!(d BuiltinMDProp::DebugSourcelocation);
		do_prop!(d BuiltinMDProp::DebugSourcechecksum);
		do_prop!(d BuiltinMDProp::DebugDwarfversion);
		do_prop!(d BuiltinMDProp::DebugTypenames);
		// TODO: !debug/<custom>
		// TODO: !<custom>
		
		// ===== then generate and join ir if need be =====
		
		let mut ir = IR::new();

		macro_rules! emit {
			($ir:expr) => {
				if !$ir.is_empty() {
					ir += &$ir;
					ir.push_str(ENDL);
				}
			};
		}

		if before_contents {
			if !m.is_empty() || !ms.is_empty() || !mp.is_empty() {
				ir += &generate_ir_comment("module metadata", true);
			}

			emit!(m);
			emit!(ms);
			emit!(mp);
		} else {
			if !t.is_empty() { ir += &generate_ir_comment("target metadata", true); }
			emit!(t);

			if !d.is_empty() { ir += &generate_ir_comment("debug info metadata", true); }
			emit!(d);
		}

		// return it
		ir
	}

	/// Generates the complete IR for the module.
	pub fn generate_ir(&mut self) -> IR {
		let mut ir = IR::new();

		// generate metadata ir first
		ir += &self.generate_metadata_ir(true);

		ir.trim().to_string()
	}
}