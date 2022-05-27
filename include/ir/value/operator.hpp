//===--- ir/value/operator.hpp ---------- C++ ---===
// This header is part of the EvIr library
// MIT License - Sjoerd Vermeulen (2022)
// For more info see https://github.com/SjVer/EvIr
//===--------------------------------------------===

#ifndef EVIR_IR_VALUE_OPERATOR_H
#define EVIR_IR_VALUE_OPERATOR_H
#define __EVIR_HEADER

#include "evir/.common.hpp"
#include "evir/ir/value/value.hpp"
#include "evir/ir/user/function.hpp"

namespace evir {

class Operator : public Value
{
protected:
	/// @cond

	static const ValueType value_type = VALUE_OPERATOR;

	/// @endcond
public:

	/// @copydoc Value::generate_ir()
	virtual String generate_ir() = 0;

	// virtual void resolve();
};

class BinaryOp : public Operator
{
public:

	typedef enum BinOpType
	{
		ADD,
		SUB,
		MUL,
		DIV,
		MOD,
		SHL,
		SHR,
		AND,
		OR,
		XOR,
	};

private:

	Value* lhs;
	Value* rhs;
	BinOpType optype;

public:

	BinaryOp(BinOpType type, Value* lhs, Value* rhs):
		optype(type), lhs(lhs), rhs(rhs) {}

	/// @copydoc Value::generate_ir()
	String generate_ir();
};

class ComparisonOp : public Operator
{
public:

	typedef enum CmpOpType
	{
		EQ,
		NEQ,
		LT,
		LTEQ,
		GT,
		GTEQ,
	};

private:

	Value* lhs;
	Value* rhs;
	CmpOpType optype;

public:

	ComparisonOp(CmpOpType type, Value* lhs, Value* rhs):
		optype(type), lhs(lhs), rhs(rhs) {}

	/// @copydoc Value::generate_ir()
	String generate_ir();
};

class CallOp : public Operator
{
	Function* callee;
	Vector<Value*> args;

public:

	CallOp(Function* callee, Vector<Value*> args):
		callee(callee), args(args) {}

	/// @copydoc Value::generate_ir()
	String generate_ir();
};

}

#endif // EVIR_IR_VALUE_OPERATOR_H