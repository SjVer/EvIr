//===--- lib.rs ------------------------ Rust ---===
// This header is part of the EvIr library
// MIT License - Sjoerd Vermeulen (2022)
// For more info see https://github.com/SjVer/EvIr
//===--------------------------------------------===

pub mod ir;

static ENDL: &str = "\n";
static TAB: &str = "    ";
// static TAB_LEN: u32 = 4;

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
		assert!($cond, "EvIr assertion failed: {}", format!($($arg)*))
	};
}