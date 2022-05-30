//===--- ir/mod.rs --------------------- Rust ---===
// This header is part of the EvIr library
// MIT License - Sjoerd Vermeulen (2022)
// For more info see https://github.com/SjVer/EvIr
//===--------------------------------------------===

mod metadata;
mod comment;
mod module;

pub use metadata::*;
pub use comment::*;
pub use module::*;

pub type IR = String;