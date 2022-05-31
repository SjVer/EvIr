//===--- lib.rs ------------------------ Rust ---===
// This header is part of the EvIr library
// MIT License - Sjoerd Vermeulen (2022)
// For more info see https://github.com/SjVer/EvIr
//===--------------------------------------------===

pub mod ir;

static ENDL: &str = "\n";
static TAB: &str = "    ";
static TAB_LEN: usize = 4;

// static LIB_NAME: &str = "evir";
static LIB_NAME_INTERNAL: &str = "evir (rust)";
#[cfg(debug_assertions)]
static LIB_VERSION: &str = "0.1.0 dev";
#[cfg(not(debug_assertions))]
static LIB_VERSION: &str = "0.1.0";

// static UNRESOLVED: &str = "<unresolved>";

#[macro_export]
macro_rules! evir_assert {
	($cond:expr, $whatwentwrong:expr) => {
		assert!($cond, "EvIr assertion failed: {}", $whatwentwrong)
	};
}

#[macro_export]
macro_rules! evir_assert_f {
	($cond:expr, $($arg:tt)*) => {
		// assert!($cond, "EvIr assertion failed: {}", format!($($arg)*))
		crate::evir_assert!($cond, format!($($arg)*))
	};
}

/// indents once or more using TAB
#[macro_export]
macro_rules! i {
	($what:expr) => {
		format!("{}{}", crate::TAB, $what)
	};

	($times:expr, $what:expr) => {
		format!("{}{}", crate::TAB.repeat($times), $what)
	};
}