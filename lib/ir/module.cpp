#include "evir/ir/module.hpp"
#include "evir/ir/comment.hpp"
#include "evir/ir/value/constant.hpp"
#include "evir/ir/value/reference.hpp"

using namespace evir;

#pragma region Constructors

Module::Module() {}

Module::Module(String name): name(name)
{
	add_metadata(new Metadata(Metadata::MD_MODULE_NAME, new MDStringValue(name)));
}

String Module::get_name()
{
	// metadata isn't set
	if(!has_metadata(Metadata::MD_MODULE_NAME)) return "";
	
	// metadata isn't a string
	Metadata* mdata = get_metadata(Metadata::MD_MODULE_NAME);
	if(!mdata->p_value->is_string()) return "";

	return ((MDStringValue*)mdata->p_value)->string;
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
	EVIR_ASSERT(!has_metadata(mdata->p_path), "Module already has metadata with similar path!");
	metadata.push_back(mdata);
}

void Module::add_metadata(Metadata::BuiltinPropertyID id, MDValue* value)
{
	EVIR_ASSERT(!has_metadata(id), "Module already has metadata with similar path!");
	add_metadata(new Metadata(id, value));
}

void Module::set_metadata(Metadata::Path path, MDValue* value)
{
	Metadata* mdata = get_metadata(path);
	EVIR_ASSERT(mdata, "Cannot set value of non-existent metadata property!");

	mdata->p_value = value;
}

void Module::set_metadata(Metadata::BuiltinPropertyID id, MDValue* value)
{
	Metadata* mdata = get_metadata(id);
	EVIR_ASSERT(mdata, "Cannot set value of non-existent metadata property!");

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

Function* Module::get_function(String name)
{
	for(auto u : users)
		if(u->type->is_function_type() && u->get_name() == name)
			return (Function*)u;

	return nullptr;
}

Function* Module::get_or_insert_function(FunctionType* type, String name)
{
	Function* func = get_function(name);

	// not found
	if(!func)
	{
		func = new Function(type, name);
		users.push_back(func);
		return func;
	}

	// wrong type
	else if(*func->type != *type) return nullptr;

	// found
	else return func;
}

// Global* Module::get_or_insert_global(Type* type, String name)
// {
// 	for(auto u : users)
// 	{
// 		if(u->get_name() != name) continue;
// 		else if(*u->type != *type) return nullptr;
// 		else return (Global*)u;
// 	}
//
// 	Global* glob = new Global(type, name);
// 	users.push_back(glob);
// 	return glob;
// }

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
	#define HANDLE_RANGE(do, name) if(do && md->p_id > Metadata::_MD_start_##name && \
								  md->p_id < Metadata::_MD_end_##name) \
							   { s_##name << md->generate_ir() << endl; continue; }

	// gather metadata ir in seperate streams
	for(auto md : metadata)
	{
		HANDLE_RANGE(before_contents, module);
		HANDLE_RANGE(before_contents, module_source);
		HANDLE_RANGE(before_contents, module_producer);

		// debug/typenames is on a new line
		if(!before_contents && md->p_id == Metadata::_MD_DEBUG_TYPENAMES) s_debug << endl;

		HANDLE_RANGE(!before_contents, target);
		HANDLE_RANGE(!before_contents, debug);

		// custom metadata follows at the end
		if(!before_contents && md->p_id == Metadata::_MD_CUSTOM_)
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
	stream << generate_ir_comment("\t" EVIR_LIB_NAME_INTERNAL " version " EVIR_LIB_VERSION " dev");
	#else
	stream << generate_ir_comment("\t" EVIR_LIB_NAME_INTERNAL " version " EVIR_LIB_VERSION);
	#endif
	stream << generate_ir_comment("\tby Sjoerd Vermeulen");
	stream << generate_ir_comment("\tMIT license (2022)");

	// return string (minus last newline)
	return stream.str().erase(stream.str().length() - 1);
}

#pragma endregion