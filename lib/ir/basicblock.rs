//===--- ir/basicblock.rs -------------- Rust ---===
// This header is part of the EvIr library
// MIT License - Sjoerd Vermeulen (2022)
// For more info see https://github.com/SjVer/EvIr
//===--------------------------------------------===

use crate::{
	ENDL, TAB, TAB_LEN,
	__evir_get_next_tmp_name, i,
	ir::{Instruction, IR, generate_ir_comment}
};

pub(crate) static mut LABEL_TMPNAMEGETTER: u32 = 0;

#[derive(Debug, Clone)]
pub struct BasicBlock {
	label: Option<String>,
	predecessors: Vec<*const BasicBlock>,
	instructions: Vec<Instruction>,
}

// state
impl BasicBlock {
	pub fn new() -> Self {
		Self {
			label: None,
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
		self.label = Some(label.to_string());
	}

	pub fn get_label(&self) -> Option<String> {
		self.label.clone()
	}

	pub(crate) fn get_ir_label(&self) -> IR {
		self.get_label().unwrap_or(__evir_get_next_tmp_name!(LABEL_TMPNAMEGETTER))
	}

	pub fn has_label(&self) -> bool {
		self.label.is_some()
	}

	pub fn remove_label(&mut self) {
		self.label = None;
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

	pub(crate) fn _add_predecessor(&mut self, pred: *const BasicBlock) {
		self.predecessors.push(pred);
	}

	pub fn generate_ir(&self) -> IR {
		let mut ir = IR::new();

		// preds comment
		if !self.predecessors.is_empty() {
			let preds: Vec<String> = unsafe {
				self.predecessors.as_bb_refs().iter()
					.map(|p| p.get_ir_label())
					.collect()
			};
			ir += &generate_ir_comment(format!("preds: {}", preds.join(", ")), false);
		}

		// label
		let label = self.get_ir_label();
		ir += &i!(format!("#{} ->", label));

		// label padding
		if label.len() < TAB_LEN { ir += &TAB.repeat(2); }
		else if label.len() >= 2 * TAB_LEN { ir.push(' '); }
		else { ir += TAB; }

		// instructions
		let mut instrs = vec![];
		for i in &self.instructions {
			// i.resolve();
			instrs.push(i.generate_ir());
		}
		ir += &instrs.join(&TAB.repeat(4));
		if self.instructions.is_empty() { ir.push_str(ENDL); }

		// return
		ir
	}
}

// helper to turn Vec<*const BasicBlock> into Vec<&BasicBlock>
// eliminating all invalid pointers
trait _H {
	unsafe fn as_bb_refs(&self) -> Vec<&BasicBlock>;
}
impl _H for Vec<*const BasicBlock> {
	unsafe fn as_bb_refs(&self) -> Vec<&BasicBlock> {
		let mut r = vec![];
		
		for b in self {
			if let Some(b) = b.as_ref() {
				r.push(b);
			}
		}

		r
	}
}