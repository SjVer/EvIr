#include "ir/module.hpp"

using namespace eviir;

#pragma endregion
#pragma region Constructors

Module::Module(string name): name(name)
{
	add_metadata(new Metadata(Metadata::META_MODULE_NAME, new StringValue(name)));
	add_metadata(new Metadata(Metadata::META_MODULE_ENTRYPOINT, new ReferenceValue("main")));

	add_metadata(new Metadata(Metadata::META_MODULE_SOURCE_FILENAME, new StringValue("test.evi")));
	add_metadata(new Metadata(Metadata::META_MODULE_SOURCE_DIRECTORY, new StringValue("test/")));

	add_metadata(new Metadata(Metadata::META_MODULE_PRODUCER_NAME, new StringValue("evi (official)")));
	add_metadata(new Metadata(Metadata::META_MODULE_PRODUCER_VERSION, new StringValue("0.0.1")));
	add_metadata(new Metadata(Metadata::META_MODULE_PRODUCER_TYPE, new StringValue("compiler")));
}

#pragma endregion
#pragma region Metadata manipulation

bool Module::has_metadata(Metadata::path path)
{
	// get it, and if it's null it isn't found
	return (bool)get_metadata(path);
}

void Module::add_metadata(Metadata* mdata)
{
	ASSERT(!has_metadata(mdata->p_path), "Module already has metadata with similar path!");
	metadata.push_back(mdata);
}

void Module::set_metadata(Metadata::path path, Value* value)
{
	Metadata* mdata = get_metadata(path);
	ASSERT(mdata, "Cannot set value of non-existent metadata property!");

	mdata->p_value = value;
}

Metadata* Module::get_metadata(Metadata::path path)
{
	// TODO?: improve?

	for(auto md : metadata) if(md->p_path == path) return md;
	return nullptr;
}

#pragma endregion
#pragma region IR generation

string Module::generate_ir_comment(string text, bool header)
{
	int max_sentence_size = header ? __IR_HCOMMENT_LENGTH - 2 * __IR_HCOMMENT_MIN_SURROUND - 2
								   : __IR_COMMENT_LENGTH;

	// separate text in sentences
	vector<string> sentences = vector<string>();
	string sentence = text;
	while(sentence.length() > max_sentence_size)
	{
		// too long, find whitespace to split sentences
		int i;
		for(i = max_sentence_size - 1; i > 0 && sentence[i] != ' '; i--) {}

		sentences.push_back(tools::trim_copy(sentence.substr(0, i)));
		sentence.erase(0, i);
	}
	sentences.push_back(tools::trim_copy(sentence));

	// generate comment(s)
	if(header)
	{
		if(sentences.size() == 1)
		{
			sstream stream = sstream();

			int len = sentence.length() + 2;
			int surround_chars = __IR_HCOMMENT_LENGTH - len;
			int start_surround_chars = surround_chars / 2;
			int end_surround_chars = surround_chars - start_surround_chars;

			stream << "; " << string(start_surround_chars, __IR_HCOMMENT_SURROUND_CHAR);
			stream << " " << sentence << " ";
			stream << string(end_surround_chars, __IR_HCOMMENT_SURROUND_CHAR) << std::endl;

			return stream.str();
		}
		else
		{
			sstream stream = sstream();
			for(const string& sentence : sentences)
			{
				int end_whitespaces = __IR_HCOMMENT_LENGTH - (
					2 * __IR_HCOMMENT_MIN_SURROUND + 1 + sentence.length()); 

				stream << "; " << string(__IR_HCOMMENT_MIN_SURROUND, __IR_HCOMMENT_SURROUND_CHAR);
				stream << ' ' << sentence << string(end_whitespaces, ' ');
				stream << string(__IR_HCOMMENT_MIN_SURROUND, __IR_HCOMMENT_SURROUND_CHAR);
				stream << std::endl;
			}
			return stream.str();
		}
	}
	else
	{
		if(sentences.size() == 1) return "; " + sentence;
		else
		{
			sstream stream = sstream();
			for(const string& sentence : sentences)
			{
				stream << "; " << sentence;
				stream << std::endl;
			}
			return stream.str();
		}
	}
}

string Module::generate_metadata_ir()
{
	sstream stream = sstream();


	// module/... metadata
	stream << generate_ir_comment("module metadata", true);
	for(auto md : metadata) if (
			md->p_type > Metadata::_META_start_module &&
			md->p_type < Metadata::_META_end_module)
		stream << md->generate_ir() << std::endl;

	// module/source metadata
	stream << std::endl;
	for(auto md : metadata) if (
			md->p_type > Metadata::_META_start_module_source &&
			md->p_type < Metadata::_META_end_module_source)
		stream << md->generate_ir() << std::endl;

	// module/producer metadata
	stream << std::endl;
	for(auto md : metadata) if (
			md->p_type > Metadata::_META_start_module_producer &&
			md->p_type < Metadata::_META_end_module_producer)
		stream << md->generate_ir() << std::endl;

	stream << std::endl;
	return stream.str(); 
}

string Module::generate_ir()
{
	sstream stream = sstream();

	// generate metata ir
	stream << generate_metadata_ir();

	// return string
	return stream.str();
}