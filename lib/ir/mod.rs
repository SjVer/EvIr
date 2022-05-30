//===--- ir/mod.rs --------------------- Rust ---===
// This header is part of the EvIr library
// MIT License - Sjoerd Vermeulen (2022)
// For more info see https://github.com/SjVer/EvIr
//===--------------------------------------------===

mod comment;
mod metadata;
mod module;
mod user;

pub use comment::*;
pub use metadata::*;
pub use module::*;
pub use user::*;

pub type IR = String;