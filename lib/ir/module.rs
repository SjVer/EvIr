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
		metadata::*,
		User, UserT
	}
};

/// A single stand-alone IR module. \
/// Has the following metadata by default:
/// - [!module/name][`BuiltinMDProp::ModuleName`]
/// - [!debug/generate][`BuiltinMDProp::DebugGenerate`]
pub struct Module<'a> {
	metadata: Vec<Metadata>,
	users: Vec<User<'a>>,	
}

// misc stuff
impl Module<'_> {
	pub fn new(name: impl ToString) -> Self {
		let mut this = Self {
			metadata: vec![],
			users: vec![],
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
impl Module<'_> {
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
impl Module<'_> {
	fn generate_metadata_ir(&mut self, before_contents: bool) -> IR {
		// ===== first gather ir for each part seperatly =====
		
		let mut m  = IR::new(); // !module/...
		let mut ms = IR::new(); // !module/source/...
		let mut mp = IR::new(); // !module/producer/...
		let mut t  = IR::new(); // !target/...
		let mut d  = IR::new(); // !debug/...
		let mut c  = IR::new(); // !...

		macro_rules! do_prop {
			($what:ident $path:ident) => {
				if let Some(md) = self.get_metadata(BuiltinMDProp::$path) {
					$what += &md.generate_ir();
				}
			};
		}
		macro_rules! do_cust {
			($what:ident $path:ident) => {
				for md in &self.metadata {
					if let MDPath::Custom(s, ..) = &md.path {
						if s == &CustomMDProp::$path {
							$what += &md.generate_ir();
						}
					}
				} 
			};
		}

		do_prop!(m ModuleName);
		do_prop!(m ModuleEntrypoint);

		do_prop!(ms ModuleSourceFilename);
		do_prop!(ms ModuleSourceDirectory);
		do_prop!(ms ModuleSourceLanguage);
		do_cust!(ms ModuleSource);

		do_prop!(mp ModuleProducerName);
		do_prop!(mp ModuleProducerVersion);
		do_prop!(mp ModuleProducerType);
		do_cust!(mp ModuleProducer);

		do_cust!(m Module);

		do_prop!(t TargetTriple);
		do_prop!(t TargetCPU);
		do_prop!(t TargetDatalayout);
		do_prop!(t TargetOptimization);
		do_cust!(t Target);

		do_prop!(d DebugGenerate);
		do_prop!(d DebugIncludesource);
		do_prop!(d DebugSourcelocation);
		do_prop!(d DebugSourcechecksum);
		do_prop!(d DebugDwarfversion);
		do_prop!(d DebugTypenames);
		do_cust!(d Debug);

		do_cust!(c Custom);
		
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
			
			emit!(c);
		}

		// return it
		ir
	}

	/// Generates the complete IR for the module.
	pub fn generate_ir(&mut self) -> IR {
		let mut ir = IR::new();

		// generate metadata ir first
		ir += &self.generate_metadata_ir(true);

		// generate users' ir
		ir += &generate_ir_comment("module contents", true);
		ir.push('\n');
		for u in &self.users {
			ir += &u.generate_ir();
			ir.push('\n');
		}

		// return the ir
		ir.trim().to_string()
	}
}