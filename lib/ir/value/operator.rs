//===--- ir/value/operator.rs ---------- Rust ---===
// This header is part of the EvIr library
// MIT License - Sjoerd Vermeulen (2022)
// For more info see https://github.com/SjVer/EvIr
//===--------------------------------------------===

use crate::ir::{IR, Value};

#[derive(Debug, Clone)]
pub enum Operator {
	Easy(Easy),
	Hard(Hard),
	Comp(Comp),
	Call(Call),
}

type OpV = Box<Value>;

pub trait Op {
	fn to_operator(self) -> Self;
	fn is_constant(&self) -> bool { false }
	fn unpack_operands(&self) -> Vec<&OpV>;
	fn generate_ir(&self) -> IR;
}

impl Op for Operator {
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

// ========== Operator types ==========

#[derive(Debug, Clone)]
pub enum Easy {
	SHL(OpV, OpV),
	SHR(OpV, OpV),
	Or (OpV, OpV),
	XOr(OpV, OpV),
	And(OpV, OpV),
	// NEG(OpV),
	// NZ (OpV),
}

impl Op for Easy {
	fn is_constant(&self) -> bool {
		match self {
			Self::SHL(l, r) |
			Self::SHR(l, r) |
			Self::Or (l, r) |
			Self::XOr(l, r) |
			Self::And(l, r) =>
				l.is_constant() && r.is_constant(),
		}
	}

	fn unpack_operands(&self) -> Vec<&OpV> {
		match self {
			Self::SHL(l, r) |
			Self::SHR(l, r) |
			Self::Or (l, r) |
			Self::XOr(l, r) |
			Self::And(l, r) =>
				vec![l, r],
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
		};

		format!("${} {}", operator, operands.join(" "))
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

#[derive(Debug, Clone)]
pub struct Call {
	callee: String, // &mut Function
	arguments: Vec<Value>,
}

impl Op for Call {
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