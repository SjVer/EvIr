//===--- ir/metadata/path.rs ----------- Rust ---===
// This header is part of the EvIr library
// MIT License - Sjoerd Vermeulen (2022)
// For more info see https://github.com/SjVer/EvIr
//===--------------------------------------------===

use convert_case::{Casing, Case::Lower};

/// todo: doc
#[derive(Debug, Clone, PartialEq)]
pub enum MDPath {
	Builtin(BuiltinMDProp),
	Custom(CustomMDProp, Vec<String>),
}

impl ToString for MDPath {
	fn to_string(&self) -> String {
		match self {
			Self::Builtin(p) => {
				format!("{:?}", p).to_case(Lower)
					.split(' ').collect::<Vec<&str>>()
					.join("/")
			},
			Self::Custom(p, r) => {
				let n = format!("{:?}", p).to_case(Lower);
				let mut p = n.split(' ').collect::<Vec<&str>>();
				p.extend(r.iter().map(|p| p.as_str()));
				p.join("/")
			},
		}
	}
}

// properties

#[derive(Debug, Clone, PartialEq)]
pub enum BuiltinMDProp {
	/// metadata property path: `!module/name`
	ModuleName,
	/// metadata property path: `!module/entrypoint`
	ModuleEntrypoint,

	/// metadata property path: `!module/source/filename`
	ModuleSourceFilename,
	/// metadata property path: `!module/source/directory`
	ModuleSourceDirectory,
	/// metadata property path: `!module/source/language`
	ModuleSourceLanguage,

	/// metadata property path: `!module/producer/name`
	ModuleProducerName,
	/// metadata property path: `!module/producer/version`
	ModuleProducerVersion,
	/// metadata property path: `!module/producer/type`
	ModuleProducerType,

	/// metadata property path: `!target/triple`
	TargetTriple,
	/// metadata property path: `!target/cpu`
	TargetCPU,
	/// metadata property path: `!target/datalayout`
	TargetDatalayout,
	/// metadata property path: `!target/optimization`
	TargetOptimization,

	/// metadata property path: `!debug/generate`
	DebugGenerate,
	/// metadata property path: `!debug/includesource`
	DebugIncludesource,
	/// metadata property path: `!debug/sourcelocation`
	DebugSourcelocation,
	/// metadata property path: `!debug/sourcechecksum`
	DebugSourcechecksum,
	/// metadata property path: `!debug/dwarfversion`
	DebugDwarfversion,
	/// metadata property path: `!debug/typenames`
	DebugTypenames,
}

#[derive(Debug, Clone, PartialEq)]
pub enum CustomMDProp {
	/// metadata property path: `!module/...`
	Module,
	/// metadata property path: `!module/source/...`
	ModuleSource,
	/// metadata property path: `!module/producer/...`
	ModuleProducer,
	/// metadata property path: `!target/...`
	Target,
	/// metadata property path: `!debug/...`
	Debug,
	/// metadata property path: `!...`
	Custom,
}

// trait

#[doc(hidden)]
pub trait __Tomdpath {
	fn to_mdpath(self) -> MDPath;
}

impl __Tomdpath for MDPath {
	fn to_mdpath(self) -> MDPath {
		self
	}
}

impl __Tomdpath for BuiltinMDProp {
	fn to_mdpath(self) -> MDPath {
		MDPath::Builtin(self)
	}
}

impl __Tomdpath for CustomMDProp {
	fn to_mdpath(self) -> MDPath {
		MDPath::Custom(self, vec![])
	}
}

impl __Tomdpath for Vec<String> {
	fn to_mdpath(self) -> MDPath {
		MDPath::Custom(CustomMDProp::Custom, self)
	}
}