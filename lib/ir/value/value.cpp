#include "evir/ir/value/value.hpp"
#include "evir/ir/metadata/mdvalue.hpp"

using namespace evir;

Value::operator MDIRValue()
{
	return MDIRValue(this);
}