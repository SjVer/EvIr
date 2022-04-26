#include "evir/ir/basicblock.hpp"
#include "evir/ir/comment.hpp"

using namespace evir;

void BasicBlock::set_label(String label_)
{
	ASSERT(label_.length(), "invalid label!");
	label = label_;
	label_set = true;
}

String BasicBlock::get_label()
{
	if(label_set) return label;
	else if(tmp_label_getter)
	{
		set_label(tmp_label_getter());
		return label;
	}
	else return UNRESOLVED;
}


void BasicBlock::add_predecessor(BasicBlock* pred)
{
	ASSERT(pred, "invalid predecessor block!");
	preds.push_back(pred);
	preds.shrink_to_fit();
}

String BasicBlock::generate_ir()
{
	SStream stream = SStream();

	// ; preds: ...
	if(preds.size())
	{
		SStream predscomment = SStream();
		predscomment << "preds: ";

		// ASSERT label
		for(auto pred : preds)
		{
			predscomment << pred->get_label();
			if(pred != preds.back()) predscomment << ", ";
		}

		stream << tab << generate_ir_comment(predscomment.str());
	}

	// label
	stream << tab << '#' << get_label() << " ->";

	// label padding
	if(label.length() < tab_len) stream << tab << tab;
	else if(label.length() >= 2 * tab_len) stream << " ";
	else stream << tab;

	// instructions padding
	String indent = String() + tab + tab + tab + tab;

	// instructions
	for(auto i : instructions)
	{
		if(i != instructions.front()) stream << indent;
		i->resolve();
		stream << i->generate_ir();
	}
	if(instructions.size() == 0) stream << endl;

	return stream.str();
}