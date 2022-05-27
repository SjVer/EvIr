#include "evir/ir/value/operator.hpp"

using namespace evir;

String BinaryOp::generate_ir()
{
	String op;
	switch (optype)
	{
		case ADD: op = "add"; break;
		case SUB: op = "sub"; break;
		case MUL: op = "mul"; break;
		case DIV: op = "div"; break;
		case MOD: op = "mod"; break;
		case SHL: op = "shl"; break;
		case SHR: op = "shr"; break;
		case AND: op = "and"; break;
		case OR : op = "or"; break;
		case XOR: op = "xor"; break;
		default: EVIR_ASSERT(false, "during binary operator IR generation");
	}

	return '$' + op + ' ' + lhs->generate_ir() + ' ' + rhs->generate_ir();
}

String ComparisonOp::generate_ir()
{
	String op = "cmp";
	switch (optype)
	{
		case EQ  : op += "eq"; break;
		case NEQ : op += "neq"; break;
		case LT  : op += "l"; break;
		case LTEQ: op += "le"; break;
		case GT  : op += "g"; break;
		case GTEQ: op += "ge"; break;
		default: EVIR_ASSERT(false, "during comparison operator IR generation");
	}

	return '$' + op + ' ' + lhs->generate_ir() + ' ' + rhs->generate_ir();
}

String CallOp::generate_ir()
{
	String ir = "$call %" + callee->get_name();
	for(Value* v : args) ir += ' ' + v->generate_ir();
	
	return ir;
}