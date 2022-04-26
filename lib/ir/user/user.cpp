#include "evir/ir/user/user.hpp"
#include "evir/ir/comment.hpp"

using namespace evir;

String User::generate_props_comment()
{
	// if(properties.shrink_to_fit(), properties.size())
	{
		SStream comment = SStream();
		comment << "props: ";

		for(auto prop : properties)
		{
			// comment << prop;
			// if(prop != properties.back()) comment << ", ";
			comment << prop << ", ";
		}

		// built-in properties
		comment << (is_defined() ? "defined" : "declared");

		return generate_ir_comment(comment.str());
	}
	return "";
}