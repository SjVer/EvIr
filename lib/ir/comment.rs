//===--- ir/comment.rs ----------------- Rust ---===
// This header is part of the EvIr library
// MIT License - Sjoerd Vermeulen (2022)
// For more info see https://github.com/SjVer/EvIr
//===--------------------------------------------===

use super::IR;

const CLENGHT: usize = 49;
const HLENGHT: usize = 50;
const HMINSURR: usize = 3;

pub fn generate_ir_comment(text: impl ToString, is_header: bool) -> IR {
	let max_sentence_size: usize = if is_header {
		HLENGHT - 2 * HMINSURR - 2
	} else { CLENGHT };

	// separate text in sentences
	let mut sentences = Vec::<IR>::new();
	let mut sentence = text.to_string();

	while sentence.len() > max_sentence_size {
		// too long! split and go on
		let (first, rest) = sentence.split_once(char::is_whitespace)
			.unwrap_or(("", &sentence));

		sentences.push(first.to_string());
		sentence = rest.trim_matches(' ').to_string();
	}
	sentences.push(sentence.trim_matches(' ').to_string());

	// generate comment
	let mut ir = IR::new();
	for s in sentences {
		if is_header {
			ir += &format!("; {:=^1$}\n", format!(" {} ", s), HLENGHT)
		} else {
			ir += &format!("; {}\n", s)
		}
	}
	ir
}