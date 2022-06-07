//===--- frontend/diagnostic.rs -------- Rust ---===
// This header is part of the EvIr library
// MIT License - Sjoerd Vermeulen (2022)
// For more info see https://github.com/SjVer/EvIr
//===--------------------------------------------===

use std::io::{Write, stderr};
use yansi::Color::Red;

pub struct Diagnostic {
	message: String
}

impl Diagnostic {
	pub fn new(message: impl ToString) -> Self {
		Self {
			message: message.to_string(),
		}
	}

	pub fn dispatch(&self) {
		writeln!(stderr(), "{}: {}", Red.paint("Error").bold(), self.message)
			.expect("Failed to write to stderr");
	}
}