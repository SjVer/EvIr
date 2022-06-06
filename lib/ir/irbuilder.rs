//===--- ir/irbuilder.rs --------------- Rust ---===
// This header is part of the EvIr library
// MIT License - Sjoerd Vermeulen (2022)
// For more info see https://github.com/SjVer/EvIr
//===--------------------------------------------===

use crate::ir::types::*;

pub struct IRBuilder {

}

// state stuff
impl IRBuilder {
	pub fn new() -> Self {
		Self {

		}
	}

}

// static stuff
macro_rules! _ty_getter {
	($name:ident, $type:expr) => {
		pub fn $name (&self) -> Type {
			$type
		}
	};
}
impl IRBuilder {
	_ty_getter!{get_bool_type, Type::Integer(IntegerType::new(false, 1))}
	_ty_getter!{get_i8_type, Type::Integer(IntegerType::new(true, 8))}
	_ty_getter!{get_u8_type,  Type::Integer(IntegerType::new(false, 8))}
	_ty_getter!{get_i16_type, Type::Integer(IntegerType::new(true, 16))}
	_ty_getter!{get_u16_type, Type::Integer(IntegerType::new(false, 16))}
	_ty_getter!{get_i32_type, Type::Integer(IntegerType::new(true, 32))}
	_ty_getter!{get_u32_type, Type::Integer(IntegerType::new(false, 32))}
	_ty_getter!{get_i64_type, Type::Integer(IntegerType::new(true, 64))}
	_ty_getter!{get_u64_type, Type::Integer(IntegerType::new(false, 64))}
	_ty_getter!{get_i128_type, Type::Integer(IntegerType::new(true, 128))}
	_ty_getter!{get_u128_type, Type::Integer(IntegerType::new(false, 128))}
	pub fn get_i_type(n: i32) -> Type { Type::Integer(IntegerType::new(true, n as u8)) }
	pub fn get_u_type(n: i32) -> Type { Type::Integer(IntegerType::new(false, n as u8)) }
	_ty_getter!{get_half_type, Type::Float(FloatType::new(16))}
	_ty_getter!{get_float_type, Type::Float(FloatType::new(32))}
	_ty_getter!{get_double_type, Type::Float(FloatType::new(64))}
	_ty_getter!{get_void_type, Type::Void(VoidType::new())}
}

