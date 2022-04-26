#include "ir/value/reference.hpp"

using namespace evir;

String Reference::generate_ir()
{
	if(user) return '%' + user->get_name();
	else return "%" UNRESOLVED;
}