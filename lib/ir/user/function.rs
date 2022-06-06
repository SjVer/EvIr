//===--- ir/user/function.rs ----------- Rust ---===
// This header is part of the EvIr library
// MIT License - Sjoerd Vermeulen (2022)
// For more info see https://github.com/SjVer/EvIr
//===--------------------------------------------===

use crate::{
	Ptr,
	__evir_get_next_tmp_name,
	ir::{IR, User, BasicBlock, IsType, FunctionType},
};

pub(crate) static mut FUNCTION_TMPNAMEGETTER: u32 = 0;

#[derive(Debug, Clone)]
pub struct Function {
	name: Option<String>,
	ftype: FunctionType,
	properties: Vec<String>,
	blocks: Vec<BasicBlock>,
}

impl User for Function {
	fn get_properties(&mut self) -> &mut Vec<String> {
		&mut self.properties
	}

	fn is_defined(&self) -> bool {
		!self.blocks.is_empty()
	}
}

// state stuff
impl Function {
	pub(crate) fn new(name: Option<String>, ftype: FunctionType) -> Self {
		Self {
			name,
			ftype,
			properties: vec![],
			blocks: vec![],
		}
	}

	pub fn get_name(&self) -> Option<String> {
		self.name.clone()
	}

	pub fn set_name(&mut self, name: impl ToString) {
		self.name = Some(name.to_string());
	}

	pub fn has_name(&self) -> bool {
		self.name.is_some()
	}

	pub fn get_type(&mut self) -> &mut FunctionType {
		&mut self.ftype
	}

	pub fn add_property(&mut self, property: impl ToString) {
		self.properties.push(property.to_string());
	}
}

// ir stuff
impl Function {
	pub fn append_block(&mut self, bb: BasicBlock) -> Ptr<BasicBlock> {
		self.blocks.push(bb);
		Ptr::new(self.blocks.last_mut().unwrap())
	}

	/// Generates the IR for the complete function.
	pub fn generate_ir(&mut self) -> IR {
		// props comment
		let mut ir = self.generate_properties_comment();

		// signature
		let name = self.get_name().unwrap_or(__evir_get_next_tmp_name!(FUNCTION_TMPNAMEGETTER));
		ir += &format!("{} {}", name, self.ftype.generate_ir());
		ir.push('\n');
		
		// body
		for b in &self.blocks {
			ir += &b.generate_ir();
			ir.push('\n');
		}

		// return
		ir
	}
}
