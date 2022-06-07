//===--- ir/user/local.rs -------------- Rust ---===
// This header is part of the EvIr library
// MIT License - Sjoerd Vermeulen (2022)
// For more info see https://github.com/SjVer/EvIr
//===--------------------------------------------===

use crate::ir::{__Evirmaybetmpstring, User, Type};

pub(crate) static LOCAL_TMPNAMECOUNT: usize = 0;

#[derive(Debug, Clone)]
pub struct __Evirlocal<T> {
	identifier: T,
	ltype: Type,
	properties: Vec<String>,
}

impl<T> User for __Evirlocal<T> {
	fn get_properties(&mut self) -> &mut Vec<String> {
		&mut self.properties
	}

	fn is_defined(&self) -> bool { true }
}

impl<T> __Evirlocal<T> {
	pub fn get_type(&mut self) -> &mut Type {
		&mut self.ltype
	}

	pub fn add_property(&mut self, property: impl ToString) {
		self.properties.push(property.to_string());
	}
}


pub type Local = __Evirlocal<__Evirmaybetmpstring>;

impl Local {
	pub(crate) fn new(name: Option<String>, ltype: Type) -> Self {
		Self {
			identifier: __Evirmaybetmpstring::from_option(name),
			ltype,
			properties: vec![],
		}
	}

	pub fn get_name(&self) -> Option<String> {
		self.identifier.to_option()
	}

	pub fn set_name(&mut self, name: impl ToString) {
		self.identifier.set(name.to_string());
	}

	pub fn has_name(&self) -> bool {
		self.identifier.is_set()
	}

	pub(crate) fn get_ir_name(&mut self) -> String {
		self.identifier.get(LOCAL_TMPNAMECOUNT)
	}
}

pub type Param = __Evirlocal<usize>;

impl Param {
	pub(crate) fn new(index: usize, ltype: Type) -> Self {
		Self {
			identifier: index,
			ltype,
			properties: vec![],
		}
	}

	pub fn get_index(&self) -> usize {
		self.identifier
	}

	pub(crate) fn get_ir_name(&mut self) -> String {
		self.identifier.to_string()
	}
}
