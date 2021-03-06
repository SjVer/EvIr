//===--- lib.rs ------------------------ Rust ---===
// This header is part of the EvIr library
// MIT License - Sjoerd Vermeulen (2022)
// For more info see https://github.com/SjVer/EvIr
//===--------------------------------------------===

pub(crate) mod frontend;
pub mod ir;
pub mod passes;

// ============= Statics =============

static ENDL: &str = "\n";
static TAB: &str = "    ";
static TAB_LEN: usize = 4;

// static LIB_NAME: &str = "evir";
static LIB_NAME_INTERNAL: &str = "evir (rust)";
#[cfg(debug_assertions)]
static LIB_VERSION: &str = "0.1.0 dev";
#[cfg(not(debug_assertions))]
static LIB_VERSION: &str = "0.1.0";

static UNRESOLVED: &str = "<unresolved>";

// ============= Types ==============

#[derive(Debug, Clone)]
pub struct Ptr<T> { ptr: *mut T }

impl<T> Ptr<T> {
	pub fn new(t: &T) -> Self {
		Self {
			ptr: (t as *const T) as *mut T
		}
	}

	pub fn null() -> Self {
		Self {
			ptr: 0 as *mut T
		}
	}

	pub fn is_null(&self) -> bool {
		unsafe {
			self.ptr.is_null() || matches!(self.ptr.as_ref(), None)
		}
	}

	pub fn as_ref(&self) -> &mut T {
		match unsafe { self.ptr.as_mut() } {
			Some(t) => t,
			None => {
				evir_assert!(false, "Internal dereference failure.");
				unreachable!()
			}
		}
	}
}

// ============= Macros =============

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

#[doc(hidden)]
#[macro_export]
macro_rules! __i {
	($what:expr) => {
		format!("{}{}", crate::TAB, $what)
	};

	($times:expr, $what:expr) => {
		format!("{}{}", crate::TAB.repeat($times), $what)
	};
}