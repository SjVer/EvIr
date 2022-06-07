//===--- ir/basicblock.rs -------------- Rust ---===
// This header is part of the EvIr library
// MIT License - Sjoerd Vermeulen (2022)
// For more info see https://github.com/SjVer/EvIr
//===--------------------------------------------===

use crate::{
	Ptr,
	ENDL, TAB, TAB_LEN, i,
	ir::{__Evirmaybetmpstring, Instruction, IR, generate_ir_comment}
};

pub(crate) static LABEL_TMPNAMECOUNT: usize = 1;

#[derive(Debug, Clone)]
pub struct BasicBlock {
	label: __Evirmaybetmpstring,
	predecessors: Vec<Ptr<BasicBlock>>,
	instructions: Vec<Instruction>,
}

// state
impl BasicBlock {
	pub fn new() -> Self {
		Self {
			label: __Evirmaybetmpstring::Unset,
			predecessors: vec![],
			instructions: vec![],
		}
	}

	pub fn new_labeled(label: impl ToString) -> Self {
		let mut this = Self::new();
		this.set_label(label);
		this
	}

	pub fn set_label(&mut self, label: impl ToString) {
		self.label.set(label.to_string());
	}

	pub fn get_label(&self) -> Option<String> {
		self.label.to_option()
	}

	pub(crate) fn get_ir_label(&mut self) -> IR {
		self.label.get(LABEL_TMPNAMECOUNT)
	}

	pub fn has_label(&self) -> bool {
		self.label.is_set()
	}

	pub fn remove_label(&mut self) {
		self.label.unset();
	}

	pub fn get_last(&self) -> Option<&Instruction> {
		self.instructions.last()
	}
}

// instruction stuff
impl BasicBlock {
	pub fn is_terminated(&self) -> bool {
		match self.instructions.last() {
			Some(i) => i.is_terminator(),
			None => false,
		}
	}

	pub(crate) fn _add_predecessor(&mut self, pred: &BasicBlock) {
		self.predecessors.push(Ptr::new(pred));
	}

	/// Appends the given [`Instruction`] at the end.
	pub fn append(&mut self, inst: Instruction) {
		self.instructions.push(inst);
	}

	pub fn generate_ir(&mut self) -> IR {
		let mut ir = IR::new();

		// preds comment
		if !self.predecessors.is_empty() {
			let preds: Vec<String> = self.predecessors.iter()
					.map(|p| p.as_ref().get_ir_label())
					.collect();
			ir += &generate_ir_comment(format!("preds: {}", preds.join(", ")), false);
		}

		// label
		let label = self.get_ir_label();
		ir += &i!(format!("#{} ->", label));

		// label padding
		if label.len() + 4 >= 3 * TAB_LEN { ir.push(' '); }
		else {
			let mut l = label.len() + 4;
			while l < 3 * TAB_LEN {
				l += 1;
				ir.push(' ');
			}
		}

		// instructions
		let mut instrs = vec![];
		for i in &self.instructions {
			// i.resolve();
			instrs.push(i.generate_ir() + ENDL);
		}
		ir += &instrs.join(&TAB.repeat(4));
		if self.instructions.is_empty() { ir.push_str(ENDL); }

		// return
		ir
	}
}