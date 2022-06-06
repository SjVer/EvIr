//===--- ir/irbuilder.rs --------------- Rust ---===
// This header is part of the EvIr library
// MIT License - Sjoerd Vermeulen (2022)
// For more info see https://github.com/SjVer/EvIr
//===--------------------------------------------===

use crate::{
	Ptr, ENDL,
	evir_assert_f,
	ir::{
		BasicBlock, Instruction,
		Value, ToValue, Constant, Operator,
		types::*,
	},
};

pub struct IRBuilder {
	insert_block: Ptr<BasicBlock>,
}

// state stuff
impl IRBuilder {
	pub fn new() -> Self {
		Self {
			insert_block: Ptr::null()
		}
	}

	pub fn set_insert_block(&mut self, bb: Ptr<BasicBlock>) {
		self.insert_block = bb;
	}

	pub fn last_instruction(&self) -> Option<&Instruction> {
		self.insert_block.deref().
	}

	/// Appends the given [`Instruction`] at the end of the
	/// current [`BasicBlock`].
	pub fn append(&self, inst: Instruction) {
		self.insert_block.deref().append(inst);
	}
}

// building
impl IRBuilder {
	pub fn append_comment(&self, text: impl ToString) {
		for s in text.to_string().split(ENDL) {
			// just don't touch `Instruction::__Cx01mnt` pls :)
			self.append(Instruction::__Cx01mnt(s.to_string()))
		}
	}

	pub fn build_disp(&self, value: impl ToValue) -> &Instruction {
		self.append(Instruction::Disp(value.to_value()));
		self.insert_block.deref().last_instruction()
	}
}

// static stuff
macro_rules! _type_getter {
	($name:ident, $type:expr) => {
		pub fn $name (&self) -> Type {
			$type
		}
	};
}
impl IRBuilder {
	_type_getter!{get_bool_type, Type::Integer(IntegerType::new(false, 1))}
	_type_getter!{get_i8_type, Type::Integer(IntegerType::new(true, 8))}
	_type_getter!{get_u8_type,  Type::Integer(IntegerType::new(false, 8))}
	_type_getter!{get_i16_type, Type::Integer(IntegerType::new(true, 16))}
	_type_getter!{get_u16_type, Type::Integer(IntegerType::new(false, 16))}
	_type_getter!{get_i32_type, Type::Integer(IntegerType::new(true, 32))}
	_type_getter!{get_u32_type, Type::Integer(IntegerType::new(false, 32))}
	_type_getter!{get_i64_type, Type::Integer(IntegerType::new(true, 64))}
	_type_getter!{get_u64_type, Type::Integer(IntegerType::new(false, 64))}
	_type_getter!{get_i128_type, Type::Integer(IntegerType::new(true, 128))}
	_type_getter!{get_u128_type, Type::Integer(IntegerType::new(false, 128))}
	pub fn get_i_type(n: i32) -> Type { Type::Integer(IntegerType::new(true, n as u8)) }
	pub fn get_u_type(n: i32) -> Type { Type::Integer(IntegerType::new(false, n as u8)) }
	_type_getter!{get_half_type, Type::Float(FloatType::new(16))}
	_type_getter!{get_float_type, Type::Float(FloatType::new(32))}
	_type_getter!{get_double_type, Type::Float(FloatType::new(64))}
	_type_getter!{get_void_type, Type::Void(VoidType::new())}

	pub fn get_true(&self) -> Value { Value::Constant(Constant::Integer(1)) }
	pub fn get_false(&self) -> Value { Value::Constant(Constant::Integer(0)) }
	pub fn get_int(&self, int: i64) -> Value {
		//! Implicitly returns `$neg int` instead of `int` if
		//! `int` is negative.
		if int >= 0 {
			Value::Constant(Constant::Integer(int as u64))
		} else {
			Value::operator(Operator::neg(Constant::Integer(int.abs() as u64)))
		}
	}
	pub fn get_char(&self, chr: char) -> Value {
		//! panics if `chr` isn't a valid ascii character
		evir_assert_f!(chr.is_ascii(), "character {:?} is not a valid ASCII character", chr);

		let mut b = [0; 2];
		chr.encode_utf8(&mut b);
		Value::Constant(Constant::Character(b[0]))
	}
	pub fn get_float(&self, val: f64) -> Value { Value::Constant(Constant::Float(val)) }
	pub fn get_string(&self, string: String) -> Value { Value::Constant(Constant::String(string)) }
}

