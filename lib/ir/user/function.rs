//===--- ir/user/function.rs ----------- Rust ---===
// This header is part of the EvIr library
// MIT License - Sjoerd Vermeulen (2022)
// For more info see https://github.com/SjVer/EvIr
//===--------------------------------------------===

use super::{
	UserT,
	super::IR,
};

pub struct Function {
	// blocks: Vec<BasicBlock>,
}

// user stuff
impl UserT for Function {
	fn is_defined(&self) -> bool {
		// !self.blocks.is_empty()
		false
	}

	fn generate_ir(&self) -> IR {
		// properties
		let mut ir = self.generate_properties_comment();

		// signature
		// ir += &format!("{} {}\n", self.get_name(), self.)

		ir
	}
}