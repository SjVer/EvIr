#if !defined(COMPILING) && __INCLUDE_LEVEL__ == 1
#	error "including this file is not allowed."
#endif

#ifndef EVIR__STATICS_PCH_H
#define EVIR__STATICS_PCH_H

#include "ir/type.hpp"

namespace evir
{
	#pragma region standard types
	#define STATIC_TYPE(cn, ln) extern const cn static_##ln##_type;

	STATIC_TYPE(IntegerType, bool);
	STATIC_TYPE(IntegerType, uint8);
	STATIC_TYPE(IntegerType, int8);
	STATIC_TYPE(IntegerType, uint16);
	STATIC_TYPE(IntegerType, int16);
	STATIC_TYPE(IntegerType, uint32);
	STATIC_TYPE(IntegerType, int32);
	STATIC_TYPE(IntegerType, uint64);
	STATIC_TYPE(IntegerType, int64);
	STATIC_TYPE(IntegerType, uint128);
	STATIC_TYPE(IntegerType, int128);
	STATIC_TYPE(FloatType, half);
	STATIC_TYPE(FloatType, float);
	STATIC_TYPE(FloatType, double);
	STATIC_TYPE(VoidType, void);

	#undef STATIC_TYPE
	#pragma endregion
}

#endif // EVIR__STATICS_PCH_H
