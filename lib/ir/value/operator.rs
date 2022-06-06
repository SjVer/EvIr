//===--- ir/value/operator.rs ---------- Rust ---===
// This header is part of the EvIr library
// MIT License - Sjoerd Vermeulen (2022)
// For more info see https://github.com/SjVer/EvIr
//===--------------------------------------------===

use crate::ir::{IR, Value, ToValue};

#[derive(Debug, Clone)]
pub enum Operator {
	Easy(Easy),
	Hard(Hard),
	Comp(Comp),
	Call(Call),
}

macro_rules! __bin_ctor {
	($name:ident, $variant:ident, $op:ident) => {
		pub fn $name (l: impl ToValue, r: impl ToValue) -> Self {
			Self::$variant ( $variant::$op (Box::new(l.to_value()), Box::new(r.to_value()) ) )
		}
	};
}
macro_rules! __una_ctor {
	($name:ident, $variant:ident, $op:ident) => {
		pub fn $name (v: impl ToValue) -> Self {
			Self::$variant ( $variant::$op (Box::new(v.to_value())) )
		}
	};
}
impl Operator {
	__bin_ctor!{shl, Easy, SHL}
	__bin_ctor!{shr, Easy, SHR}
	__bin_ctor!{or,  Easy, Or}
	__bin_ctor!{xor, Easy, XOr}
	__bin_ctor!{and, Easy, And}
	__una_ctor!{neg, Easy, Neg}
	__bin_ctor!{add, Hard, Add}
	__bin_ctor!{sub, Hard, Sub}
	__bin_ctor!{mul, Hard, Mul}
	__bin_ctor!{div, Hard, Div}
	__bin_ctor!{rem, Hard, Mod}
	__bin_ctor!{eq,  Comp, Eq}
	__bin_ctor!{ne,  Comp, NE}
	__bin_ctor!{lt,  Comp, LT}
	__bin_ctor!{le,  Comp, LE}
	__bin_ctor!{gt,  Comp, GT}
	__bin_ctor!{ge,  Comp, GE}
}

type OpV = Box<Value>;

pub trait Op {
	fn to_operator(self) -> Operator;
	fn is_constant(&self) -> bool { false }
	fn unpack_operands(&self) -> Vec<&OpV>;
	fn generate_ir(&self) -> IR;
}

impl Op for Operator {
	fn to_operator(self) -> Operator { self }

	fn is_constant(&self) -> bool {
		match self {
			Self::Easy(e) => e.is_constant(),
			Self::Hard(e) => e.is_constant(),
			Self::Comp(e) => e.is_constant(),
			Self::Call(e) => e.is_constant(),
		}
	}
	
	fn unpack_operands(&self) -> Vec<&OpV> {
		match self {
			Self::Easy(e) => e.unpack_operands(),
			Self::Hard(e) => e.unpack_operands(),
			Self::Comp(e) => e.unpack_operands(),
			Self::Call(e) => e.unpack_operands(),
		}
	}

	fn generate_ir(&self) -> IR {
		match self {
			Self::Easy(e) => e.generate_ir(),
			Self::Hard(e) => e.generate_ir(),
			Self::Comp(e) => e.generate_ir(),
			Self::Call(e) => e.generate_ir(),
		}
	}
}
impl ToValue for Operator {
	fn to_value(self) -> Value {
		Value::Operator(self)
	}
}

// ========== Operator types ==========

#[derive(Debug, Clone)]
pub enum Easy {
	SHL(OpV, OpV),
	SHR(OpV, OpV),
	Or (OpV, OpV),
	XOr(OpV, OpV),
	And(OpV, OpV),
	Neg(OpV),
}

impl Op for Easy {
	fn to_operator(self) -> Operator {
		Operator::Easy(self)
	}

	fn is_constant(&self) -> bool {
		match self {
			Self::SHL(l, r) |
			Self::SHR(l, r) |
			Self::Or (l, r) |
			Self::XOr(l, r) |
			Self::And(l, r) =>
				l.is_constant() && r.is_constant(),
			Self::Neg(v) => v.is_constant()
		}
	}

	fn unpack_operands(&self) -> Vec<&OpV> {
		match self {
			Self::SHL(l, r) |
			Self::SHR(l, r) |
			Self::Or (l, r) |
			Self::XOr(l, r) |
			Self::And(l, r) => vec![l, r],
			Self::Neg(v) => vec![v]
		}
	}

	fn generate_ir(&self) -> IR {
		let operands: Vec<IR> = self.unpack_operands()
			.iter().map(|v| v.generate_ir())
			.collect();

		let operator = match self {
			Self::SHL(..) => "shl",
			Self::SHR(..) => "shr",
			Self::Or (..) => "or",
			Self::XOr(..) => "xor",
			Self::And(..) => "and",
			Self::Neg(..) => "neg"
		};

		format!("${} {}", operator, operands.join(" "))
	}
}
impl ToValue for Easy {
	fn to_value(self) -> Value {
		Value::Operator(Operator::Easy(self))
	}
}

#[derive(Debug, Clone)]
pub enum Hard {
	Add(OpV, OpV),
	Sub(OpV, OpV),
	Mul(OpV, OpV),
	Div(OpV, OpV),
	Mod(OpV, OpV),
}

impl Op for Hard {
	fn to_operator(self) -> Operator {
		Operator::Hard(self)
	}

	fn is_constant(&self) -> bool {
		match self {
			Self::Add(l, r) |
			Self::Sub(l, r) |
			Self::Mul (l, r) |
			Self::Div(l, r) |
			Self::Mod(l, r) =>
				l.is_constant() && r.is_constant(),
		}
	}

	fn unpack_operands(&self) -> Vec<&OpV> {
		match self {
			Self::Add(l, r) |
			Self::Sub(l, r) |
			Self::Mul(l, r) |
			Self::Div(l, r) |
			Self::Mod(l, r) =>
				vec![l, r],
		}
	}

	fn generate_ir(&self) -> IR {
		let operands: Vec<IR> = self.unpack_operands()
			.iter().map(|v| v.generate_ir())
			.collect();

		let operator = match self {
			Self::Add(..) => "add",
			Self::Sub(..) => "sub",
			Self::Mul(..) => "mul",
			Self::Div(..) => "div",
			Self::Mod(..) => "mod",
		};

		format!("${} {}", operator, operands.join(" "))
	}
}
impl ToValue for Hard {
	fn to_value(self) -> Value {
		Value::Operator(Operator::Hard(self))
	}
}

#[derive(Debug, Clone)]
pub enum Comp {
	Eq(OpV, OpV),
	NE(OpV, OpV),
	LT(OpV, OpV),
	LE(OpV, OpV),
	GT(OpV, OpV),
	GE(OpV, OpV),
}

impl Op for Comp {
	fn to_operator(self) -> Operator {
		Operator::Comp(self)
	}

	fn is_constant(&self) -> bool {
		match self {
			Self::Eq(l, r) |
			Self::NE(l, r) |
			Self::LT(l, r) |
			Self::LE(l, r) |
			Self::GT(l, r) |
			Self::GE(l, r) =>
			l.is_constant() && r.is_constant(),
		}
	}
	
	fn unpack_operands(&self) -> Vec<&OpV> {
		match self {
			Self::Eq(l, r) |
			Self::NE(l, r) |
			Self::LT(l, r) |
			Self::LE(l, r) |
			Self::GT(l, r) |
			Self::GE(l, r) =>
				vec![l, r],
		}
	}

	fn generate_ir(&self) -> IR {
		let operands: Vec<IR> = self.unpack_operands()
			.iter().map(|v| v.generate_ir())
			.collect();

		let operator = match self {
			Self::Eq(..) => "eq",
			Self::NE(..) => "neq",
			Self::LT(..) => "lt",
			Self::LE(..) => "le",
			Self::GT(..) => "gt",
			Self::GE(..) => "ge",
		};

		format!("$cmp{} {}", operator, operands.join(" "))
	}
}
impl ToValue for Comp {
	fn to_value(self) -> Value {
		Value::Operator(Operator::Comp(self))
	}
}

#[derive(Debug, Clone)]
pub struct Call {
	callee: String, // &mut Function
	arguments: Vec<Value>,
}

impl Op for Call {
	fn to_operator(self) -> Operator {
		Operator::Call(self)
	}

	fn is_constant(&self) -> bool { false }

	fn unpack_operands(&self) -> Vec<&OpV> {
		// self.arguments.clone()
		// 	.into_iter()
		// 	.map(|v| &Box::new(v))
		// 	.collect::<Vec<&OpV>>()
		vec![]
	}

	fn generate_ir(&self) -> IR {
		let args: Vec<IR> = self.arguments.iter()
			.map(|v| v.generate_ir())
			.collect();

		format!("$call %{} {}", self.callee, args.join(" "))
	}
}
impl ToValue for Call {
	fn to_value(self) -> Value {
		Value::Operator(Operator::Call(self))
	}
}
