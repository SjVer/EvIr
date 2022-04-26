#include "ir/value/value.hpp"
#include "ir/metadata/mdvalue.hpp"

using namespace evir;

Value::operator MDIRValue()
{
	return MDIRValue(this);
}