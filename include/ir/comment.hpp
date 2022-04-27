//===--- ir/comment.hpp ----------------- C++ ---===
// This header is part of the EvIr library
// MIT License - Sjoerd Vermeulen (2022)
// For more info see https://github.com/SjVer/EvIr
//===--------------------------------------------===

#ifndef EVIR_IR_COMMENT_H
#define EVIR_IR_COMMENT_H
#define __EVIR_HEADER

#include "evir/.common.hpp"

#define _IR_COMMENT_LENGTH 49
#define _IR_HCOMMENT_LENGTH 50
#define _IR_HCOMMENT_SURROUND_CHAR '='
#define _IR_HCOMMENT_MIN_SURROUND 3

namespace evir {

/// @brief Generates an IR comment
/// @param text the text of the comment
/// @param header if true, the comment is formatted as a header comment
/// @return the IR as a string (with newline)
static String generate_ir_comment(String text, bool header = false)
{
	int max_sentence_size = header ? _IR_HCOMMENT_LENGTH - 2 * _IR_HCOMMENT_MIN_SURROUND - 2
								   : _IR_COMMENT_LENGTH;

	// separate text in sentences
	Vector<String> sentences = Vector<String>();
	String sentence = text;
	while(sentence.length() > max_sentence_size)
	{
		// too long, find whitespace to split sentences
		int i;
		for(i = max_sentence_size - 1; i > 0 && sentence[i] != ' '; i--) {}

		sentences.push_back(tools::trimc(sentence.substr(0, i)));
		sentence.erase(0, i);
	}
	sentences.push_back(tools::trimc(sentence));

	// generate comment(s)
	if(header)
	{
		if(sentences.size() == 1)
		{
			SStream stream = SStream();

			int len = sentence.length() + 2;
			int surround_chars = _IR_HCOMMENT_LENGTH - len;
			int start_surround_chars = surround_chars / 2;
			int end_surround_chars = surround_chars - start_surround_chars;

			stream << "; " << String(start_surround_chars, _IR_HCOMMENT_SURROUND_CHAR);
			stream << " " << sentence << " ";
			stream << String(end_surround_chars, _IR_HCOMMENT_SURROUND_CHAR) << endl;

			return stream.str();
		}
		else
		{
			SStream stream = SStream();
			for(const String& sentence : sentences)
			{
				int end_whitespaces = _IR_HCOMMENT_LENGTH - (
					2 * _IR_HCOMMENT_MIN_SURROUND + 1 + sentence.length()); 

				stream << "; " << String(_IR_HCOMMENT_MIN_SURROUND, _IR_HCOMMENT_SURROUND_CHAR);
				stream << ' ' << sentence << String(end_whitespaces, ' ');
				stream << String(_IR_HCOMMENT_MIN_SURROUND, _IR_HCOMMENT_SURROUND_CHAR);
				stream << endl;
			}
			return stream.str();
		}
	}
	else
	{
		if(sentences.size() == 1) return "; " + sentence + endl;
		else
		{
			SStream stream = SStream();
			for(const String& sentence : sentences)
			{
				stream << "; " << sentence;
				stream << endl;
			}
			return stream.str();
		}
	}
}

}

#endif // EVIR_IR_COMMENT_H