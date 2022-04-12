#include "ir/module.hpp"

using namespace evir;

#pragma region Constructors

Module::Module(string name): name(name)
{
	add_metadata(new Metadata(Metadata::META_MODULE_NAME, new StringValue(name)));
	add_metadata(new Metadata(Metadata::META_MODULE_ENTRYPOINT, new ReferenceValue("main")));
}

#pragma endregion
#pragma region Metadata manipulation

bool Module::has_metadata(Metadata::path path)
{
	// get it, and if it's null it isn't found
	return (bool)get_metadata(path);
}

bool Module::has_metadata(Metadata::builtin_property_type type)
{
	// get it, and if it's null it isn't found
	return (bool)get_metadata(type);
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

void Module::set_metadata(Metadata::builtin_property_type type, Value* value)
{
	Metadata* mdata = get_metadata(type);
	ASSERT(mdata, "Cannot set value of non-existent metadata property!");

	mdata->p_value = value;
}

Metadata* Module::get_metadata(Metadata::path path)
{
	// TODO?: improve?

	for(auto md : metadata) if(md->p_path == path) return md;
	return nullptr;
}

Metadata* Module::get_metadata(Metadata::builtin_property_type type)
{
	// TODO?: improve?

	for(auto md : metadata) if(md->p_type == (Metadata::property_type)type) return md;
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

		sentences.push_back(tools::trimc(sentence.substr(0, i)));
		sentence.erase(0, i);
	}
	sentences.push_back(tools::trimc(sentence));

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
			stream << string(end_surround_chars, __IR_HCOMMENT_SURROUND_CHAR) << endl;

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
			sstream stream = sstream();
			for(const string& sentence : sentences)
			{
				stream << "; " << sentence;
				stream << endl;
			}
			return stream.str();
		}
	}
}

string Module::generate_metadata_ir(bool before_contents)
{
	#define STREAM(name) sstream s_##name = sstream()

	STREAM(module);
	STREAM(module_source);
	STREAM(module_producer);
	STREAM(target);
	STREAM(debug);
	STREAM(custom);

	#undef STREAM
	#define HANDLE_RANGE(do, name) if(do && md->p_type > Metadata::_META_start_##name && \
								  md->p_type < Metadata::_META_end_##name) \
							   { s_##name << md->generate_ir() << endl; continue; }

	// gather metadata ir in seperate streams
	for(auto md : metadata)
	{
		HANDLE_RANGE(before_contents, module);
		HANDLE_RANGE(before_contents, module_source);
		HANDLE_RANGE(before_contents, module_producer);

		// debug/typenames is on a new line
		if(!before_contents && md->p_type == Metadata::_META_DEBUG_TYPENAMES) s_debug << endl;

		HANDLE_RANGE(!before_contents, target);
		HANDLE_RANGE(!before_contents, debug);

		// custom metadata follows at the end
		if(!before_contents && md->p_type == Metadata::_META_CUSTOM_)
			s_custom << md->generate_ir() << endl;	
	}

	#undef HANDLE_RANGE
	#define EMPTY(name) (s_##name.str().empty())
	#define EMIT_STREAM(name) if(!EMPTY(name)) stream << s_##name.str() << endl

	// concat all streams in correct order
	sstream stream = sstream();
	if(before_contents)
	{
		// module/...
		if(!(EMPTY(module) && EMPTY(module_source) && EMPTY(module_producer)))
			stream << generate_ir_comment("module metadata", true);
		EMIT_STREAM(module);
		EMIT_STREAM(module_source);
		EMIT_STREAM(module_producer);
	}
	else
	{
		// target/...
		if(!EMPTY(target)) stream << generate_ir_comment("target metadata", true);
		EMIT_STREAM(target);

		// debug/...
		if(!EMPTY(debug)) stream << generate_ir_comment("debug info metadata", true);
		EMIT_STREAM(debug);

		// ...
		EMIT_STREAM(custom);
	}

	#undef EMPTY
	#undef EMIT_STREAM

	// clean up and return
	string ir = stream.str();
	if(*ir.end() == endl) ir.erase(ir.end() - 1);
	return ir; 
}

string Module::generate_ir()
{
	sstream stream = sstream();

	// generate metata ir before contents
	stream << generate_metadata_ir(true);
	stream << endl;
	
	stream << endl;
	stream << endl;

	// generate metata ir after contents
	stream << generate_metadata_ir(false);

	// generate credit comments
	stream << generate_ir_comment("module generated with:");
	stream << generate_ir_comment("\tlibevir version 0.0.1");
	stream << generate_ir_comment("\tby Sjoerd Vermeulen");
	stream << generate_ir_comment("\tMIT license (2022)");

	// return string
	return stream.str();
}