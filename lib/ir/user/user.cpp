#include "ir/user/user.hpp"
#include "ir/comment.hpp"

using namespace evir;

String User::generate_props_comment()
{
	if(properties.shrink_to_fit(), properties.size())
	{
		SStream comment = SStream();
		comment << "props: ";

		for(auto prop : properties)
		{
			comment << prop;
			if(prop != properties.back()) comment << ", ";
		}

		return generate_ir_comment(comment.str());
	}
	return "";
}