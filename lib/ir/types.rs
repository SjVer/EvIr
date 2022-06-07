//===--- ir/types.rs ------------------- Rust ---===
// This header is part of the EvIr library
// MIT License - Sjoerd Vermeulen (2022)
// For more info see https://github.com/SjVer/EvIr
//===--------------------------------------------===

use crate::{
	evir_assert,
	ir::IR,
};

/// IR Type
#[derive(Debug, Clone, PartialEq)]
pub enum Type {
	Integer(IntegerType),
	Float(FloatType),
	Pointer(PointerType),
	Void(VoidType),
}
impl __Istype for Type {
	fn generate_ir(&self) -> IR {
		self._generate_ir()
	}

	fn to_type(&self) -> Type {
		self.clone()	
	}
}
impl Type {
	pub fn get_pointer_to(&self) -> Self {
		Self::Pointer(PointerType::new(self.clone()))
	}

	fn _generate_ir(&self) -> IR {
		match self {
			Self::Integer(t) => t.generate_ir(),
			Self::Float(t) => t.generate_ir(),
			Self::Pointer(t) => t.generate_ir(),
			Self::Void(t) => t.generate_ir(),
		}
	}

	pub fn from(ty: impl __Istype) -> Self {
		ty.to_type()
	}
}


#[doc(hidden)]
pub trait __Istype {
	fn generate_ir(&self) -> IR;
	fn to_type(&self) -> Type;
}

macro_rules! to_type {
	($enum:ident) => {
		fn to_type(&self) -> Type {
			Type::$enum(self.clone())
		}
	};
}

// ================= subtypes =================

/// Arbitrary-width integer type
#[derive(Clone, Debug, PartialEq)]
pub struct IntegerType {
	is_signed: bool,
	bitwidth: u8,
}
impl __Istype for IntegerType {
	fn generate_ir(&self) -> IR {
		format!("i.{}.{}", if self.is_signed {'s'} else {'u'}, self.bitwidth)
	}

	to_type!{Integer}
}
impl IntegerType {
	pub fn new(is_signed: bool, bitwidth: u8) -> Self {
		evir_assert!(bitwidth >= 1, "bitwidth too small");
		evir_assert!(bitwidth as u32 <= 1 << 23, "bitwidth too large");

		Self {
			is_signed,
			bitwidth,
		}
	}
}

/// Arbitrary-width floating point number type
#[derive(Clone, Debug, PartialEq)]
pub struct FloatType {
	bitwidth: u8,
}
impl __Istype for FloatType {
	fn generate_ir(&self) -> IR {
		format!("f.{}", self.bitwidth)
	}

	to_type!{Float}
}
impl FloatType {
	pub fn new(bitwidth: u8) -> Self {
		evir_assert!(bitwidth == 16 || bitwidth == 32 || bitwidth == 64, "invalid bitwidth");
		Self { bitwidth }
	}
}

/// Pointer type
#[derive(Clone, Debug, PartialEq)]
pub struct PointerType {
	contained_type: Box<Type>,
}
impl __Istype for PointerType {
	fn generate_ir(&self) -> IR {
		format!("{}*", self.contained_type.generate_ir())
	}

	to_type!{Pointer}
}
impl PointerType {
	pub fn new(contained_type: impl __Istype) -> Self {
		Self { contained_type: Box::new(contained_type.to_type()), }
	}
}

/// Void type
#[derive(Clone, Debug, PartialEq)]
pub struct VoidType;
impl __Istype for VoidType {
	fn generate_ir(&self) -> IR {
		IR::from('v')
	}

	to_type!{Void}
}
impl VoidType {
	pub fn new() -> Self {
		VoidType {}
	}
}


/// Function type
#[derive(Clone, Debug, PartialEq)]
pub struct FunctionType {
	return_type: Box<Type>,
	parameters: Vec<Type>,
}
impl FunctionType {
	pub fn new(return_type: impl __Istype, parameters: Vec<impl __Istype>) -> Self {
		Self {
			return_type: Box::new(return_type.to_type()),
			parameters: parameters.iter().map(|t| t.to_type()).collect(),
		}
	}

	pub fn get_parameters(&self) -> &Vec<Type> {
		&self.parameters
	}

	pub fn generate_ir(&self) -> IR {
		let params: Vec<IR> = self.parameters.iter()
			.map(|t| t.generate_ir())
			.collect();
			
		format!("{} ({})", self.return_type.generate_ir(), params.join(" "))
	}
}