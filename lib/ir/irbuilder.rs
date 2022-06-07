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
		Function,
		types::*,
	}, evir_assert,
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

	pub fn get_insert_block(&self) -> &BasicBlock {
		evir_assert!(!self.insert_block.is_null(), "IRBuilder's insert block not set");
		self.insert_block.as_ref()
	}

	/// Appends the given [`Instruction`] at the end of the
	/// current [`BasicBlock`].
	pub fn append(&self, inst: Instruction) {
		evir_assert!(!self.insert_block.is_null(), "IRBuilder's insert block not set");
		self.insert_block.as_ref().append(inst);
	}
	
	/// Returns the last [`Instruction`] of the insertion block
	pub fn get_last_instruction(&self) -> Option<&Instruction> {
		if self.insert_block.is_null() {
			None
		} else {
			self.insert_block.as_ref().get_last()
		}
	}
}

// instruction building
impl IRBuilder {
	pub fn append_comment(&self, text: impl ToString) {
		for s in text.to_string().split(ENDL) {
			// just don't touch `Instruction::__Cx01mnt` pls :)
			self.append(Instruction::__Cx01mnt(s.to_string()))
		}
	}

	pub fn build_br(&self, dest: &BasicBlock) -> Option<&Instruction> {
		self.append(Instruction::Br(Ptr::new(dest)));
		self.get_last_instruction()
	}

	pub fn build_condbr(&self,
		condition: Value,
		tdest: &BasicBlock,
		fdest: Option<&BasicBlock>,
	) -> Option<&Instruction> {
		let tdest = Ptr::new(tdest);
		let fdest = fdest.map(|d| Ptr::new(d));

		self.append(Instruction::CondBr(condition, tdest, fdest));
		self.get_last_instruction()
	}

	pub fn build_ret(&self, value: Value) -> Option<&Instruction> {
		self.append(Instruction::Ret(value));
		self.get_last_instruction()
	}

	pub fn build_disp(&self, value: impl ToValue) -> Option<&Instruction> {
		self.append(Instruction::Disp(value.to_value()));
		self.get_last_instruction()
	}
}

// expression building
macro_rules! __bin_build {
	($fn:ident, $op:ident) => {
		pub fn $fn (&self, lhs: impl ToValue, rhs: impl ToValue) -> Value {
			Value::operator(Operator::$op (lhs, rhs))
		}
	};
}
macro_rules! __una_build {
	($fn:ident, $op:ident) => {
		pub fn $fn (&self, value: impl ToValue) -> Value {
			Value::operator(Operator::$op (value))
		}
	};
}
impl IRBuilder {
	__bin_build!{build_shl, shl}
	__bin_build!{build_shr, shr}
	__bin_build!{build_or, or}
	__bin_build!{build_xor, xor}
	__bin_build!{build_and, and}
	__una_build!{build_neg, neg}
	__bin_build!{build_add, add}
	__bin_build!{build_sub, sub}
	__bin_build!{build_mul, mul}
	__bin_build!{build_div, div}
	__bin_build!{build_mod, rem}
	__bin_build!{build_cmpeq, cmpeq}
	__bin_build!{build_cmpne, cmpne}
	__bin_build!{build_cmplt, cmplt}
	__bin_build!{build_cmple, cmple}
	__bin_build!{build_cmpgt, cmpgt}
	__bin_build!{build_cmpge, cmpge}
	pub fn build_call(&self, callee: &mut Function, args: Vec<impl ToValue>) -> Value {
		Value::operator(Operator::call(
			callee, args.into_iter().map(|v| v.to_value()).collect()
		))
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

