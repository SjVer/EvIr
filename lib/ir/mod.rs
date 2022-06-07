//===--- ir/mod.rs --------------------- Rust ---===
// This header is part of the EvIr library
// MIT License - Sjoerd Vermeulen (2022)
// For more info see https://github.com/SjVer/EvIr
//===--------------------------------------------===

mod basicblock;
mod comment;
mod instruction;
mod irbuilder;
mod metadata;
mod module;
mod types;
mod user;
mod value;

pub use basicblock::*;
pub use comment::*;
pub use instruction::*;
pub use irbuilder::*;
pub use metadata::*;
pub use module::*;
pub use types::*;
pub use user::*;
pub use value::*;

pub type IR = String;

#[derive(Debug, Clone)]
pub enum __Evirmaybetmpstring {
	Unset,
	Tmp(String),
	Set(String),
}

impl __Evirmaybetmpstring {
	pub fn from_option(option: Option<String>) -> Self {
		if let Some(s) = option {
			Self::Set(s)
		} else {
			Self::Unset
		}
	}

	pub fn is_set(&self) -> bool {
		matches!(self, Self::Set(_))
	}

	pub fn get(&mut self, getter: usize) -> String {
		match self {
			Self::Unset => {
				let next = crate::__evir_get_next_tmp_name!(getter);
				(*self) = Self::Tmp(next.clone());
				next
			},
			Self::Tmp(s) => s.clone(),
			Self::Set(s) => s.clone(),
		}
	}

	pub fn to_option(&self) -> Option<String> {
		if let Self::Set(s) = self {
			Some(s.clone())
		} else {
			None
		}
	}

	pub fn set(&mut self, value: String) {
		(*self) = Self::Set(value);
	}

	pub fn unset(&mut self) {
		(*self) = Self::Unset;
	}
}

pub(crate) static mut TMPNAME_COUNTERS: [u32; 8] = [0; 8];

#[macro_export]
macro_rules! __evir_reset_tmp_name_count {
	($getter:expr) => {
		unsafe {
			crate::ir::TMPNAME_COUNTERS[$getter] = 0;
		}
	};
}

#[macro_export]
macro_rules! __evir_get_next_tmp_name {
	($getter:expr) => {
		unsafe {
			crate::ir::TMPNAME_COUNTERS[$getter] += 1;
			format!("_{}", crate::ir::TMPNAME_COUNTERS[$getter] - 1)
		}
	};
}