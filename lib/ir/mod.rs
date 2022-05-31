//===--- ir/mod.rs --------------------- Rust ---===
// This header is part of the EvIr library
// MIT License - Sjoerd Vermeulen (2022)
// For more info see https://github.com/SjVer/EvIr
//===--------------------------------------------===

mod basicblock;
mod comment;
mod metadata;
mod module;
mod types;
mod user;

pub use basicblock::*;
pub use comment::*;
pub use metadata::*;
pub use module::*;
pub use types::*;
pub use user::*;

pub type IR = String;

#[macro_export]
macro_rules! __evir_reset_tmp_name_count {
	($getter:expr) => {
		unsafe { $getter = 0; }
	};
}

#[macro_export]
macro_rules! __evir_get_next_tmp_name {
	($getter:expr) => {
		unsafe { $getter += 1; format!("_{}", $getter - 1) }
	};
}