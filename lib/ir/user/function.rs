//===--- ir/user/function.rs ----------- Rust ---===
// This header is part of the EvIr library
// MIT License - Sjoerd Vermeulen (2022)
// For more info see https://github.com/SjVer/EvIr
//===--------------------------------------------===

use crate::{
	Ptr,
	ir::{__Evirmaybetmpstring, IR, User, BasicBlock, IsType, FunctionType},
};

pub(crate) static FUNCTION_TMPNAMECOUNT: usize = 0;

#[derive(Debug, Clone)]
pub struct Function {
	name: __Evirmaybetmpstring,
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
			name: __Evirmaybetmpstring::from_option(name),
			ftype,
			properties: vec![],
			blocks: vec![],
		}
	}

	pub fn get_name(&self) -> Option<String> {
		self.name.to_option()
	}

	pub fn set_name(&mut self, name: impl ToString) {
		self.name.set(name.to_string());
	}

	pub fn has_name(&self) -> bool {
		self.name.is_set()
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
		Ptr::new(self.blocks.last().unwrap())
	}

	pub(crate) fn get_ir_name(&mut self) -> String {
		self.name.get(FUNCTION_TMPNAMECOUNT)
	}

	/// Generates the IR for the complete function.
	pub fn generate_ir(&mut self) -> IR {
		// props comment
		let mut ir = self.generate_properties_comment();

		// signature
		let name = self.name.get(FUNCTION_TMPNAMECOUNT);
		ir += &format!("{} {}", name, self.ftype.generate_ir());
		ir.push('\n');
		
		// body
		for b in &mut self.blocks {
			ir += &b.generate_ir();
			ir.push('\n');
		}

		// return
		ir
	}
}
