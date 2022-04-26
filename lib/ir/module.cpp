#include "ir/module.hpp"
#include "ir/comment.hpp"
#include "ir/value/constant.hpp"
#include "ir/value/reference.hpp"

using namespace evir;

#pragma region Constructors

Module::Module(String name): name(name)
{
	add_metadata(new Metadata(Metadata::META_MODULE_NAME, new MDStringValue(name)));
	add_metadata(new Metadata(Metadata::META_MODULE_ENTRYPOINT, (MDIRValue*)new Reference()));
}

#pragma endregion
#pragma region Metadata manipulation

bool Module::has_metadata(Metadata::Path path)
{
	// get it, and if it's null it isn't found
	return (bool)get_metadata(path);
}

bool Module::has_metadata(Metadata::BuiltinPropertyID id)
{
	// get it, and if it's null it isn't found
	return (bool)get_metadata(id);
}

void Module::add_metadata(Metadata* mdata)
{
	ASSERT(!has_metadata(mdata->p_path), "Module already has metadata with similar path!");
	metadata.push_back(mdata);
}

void Module::set_metadata(Metadata::Path path, MDValue* value)
{
	Metadata* mdata = get_metadata(path);
	ASSERT(mdata, "Cannot set value of non-existent metadata property!");

	mdata->p_value = value;
}

void Module::set_metadata(Metadata::BuiltinPropertyID id, MDValue* value)
{
	Metadata* mdata = get_metadata(id);
	ASSERT(mdata, "Cannot set value of non-existent metadata property!");

	mdata->p_value = value;
}

Metadata* Module::get_metadata(Metadata::Path path)
{
	// TODO?: improve?

	for(auto md : metadata) if(md->p_path == path) return md;
	return nullptr;
}

Metadata* Module::get_metadata(Metadata::BuiltinPropertyID id)
{
	// TODO?: improve?

	for(auto md : metadata) if(md->p_id == (Metadata::PropertyID)id) return md;
	return nullptr;
}

#pragma endregion
#pragma region User manipulation

Function* Module::insert_function(FunctionType* type, String name)
{
	Function* func = new Function(type, name);
	users.push_back(func);
	return func;
}

#pragma endregion
#pragma region IR generation

String Module::generate_metadata_ir(bool before_contents)
{
	#define STREAM(name) SStream s_##name = SStream()

	STREAM(module);
	STREAM(module_source);
	STREAM(module_producer);
	STREAM(target);
	STREAM(debug);
	STREAM(custom);

	#undef STREAM
	#define HANDLE_RANGE(do, name) if(do && md->p_id > Metadata::_META_start_##name && \
								  md->p_id < Metadata::_META_end_##name) \
							   { s_##name << md->generate_ir() << endl; continue; }

	// gather metadata ir in seperate streams
	for(auto md : metadata)
	{
		HANDLE_RANGE(before_contents, module);
		HANDLE_RANGE(before_contents, module_source);
		HANDLE_RANGE(before_contents, module_producer);

		// debug/typenames is on a new line
		if(!before_contents && md->p_id == Metadata::_META_DEBUG_TYPENAMES) s_debug << endl;

		HANDLE_RANGE(!before_contents, target);
		HANDLE_RANGE(!before_contents, debug);

		// custom metadata follows at the end
		if(!before_contents && md->p_id == Metadata::_META_CUSTOM_)
			s_custom << md->generate_ir() << endl;	
	}

	#undef HANDLE_RANGE
	#define EMPTY(name) (s_##name.str().empty())
	#define EMIT_STREAM(name) if(!EMPTY(name)) stream << s_##name.str() << endl

	// concat all streams in correct order
	SStream stream = SStream();
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
	String ir = stream.str();
	if(*ir.end() == endl) ir.erase(ir.end() - 1);
	return ir; 
}

String Module::generate_ir()
{
	SStream stream = SStream();

	// generate metata ir before contents
	stream << generate_metadata_ir(true);
	
	// generate users' ir
	stream << generate_ir_comment("module contents", true);
	stream << endl;
	for(auto user : users) stream << user->generate_ir() << endl;

	// generate metata ir after contents
	stream << generate_metadata_ir(false);

	// generate credit comments
	stream << generate_ir_comment("module generated with:");
	#ifdef DEBUG
	stream << generate_ir_comment("\t" LIB_NAME_INTERNAL " version " LIB_VERSION " dev");
	#else
	stream << generate_ir_comment("\t" LIB_NAME_INTERNAL " version " LIB_VERSION);
	#endif
	stream << generate_ir_comment("\tby Sjoerd Vermeulen");
	stream << generate_ir_comment("\tMIT license (2022)");

	// return string
	return stream.str();
}

#pragma endregion