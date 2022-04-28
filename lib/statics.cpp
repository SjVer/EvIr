#include ".statics_pch.hpp"

namespace evir
{
	#pragma region standard types
	#define STATIC_TYPE(cn, ln, ...) \
		const cn static_##ln##_type = cn(__VA_ARGS__)

	STATIC_TYPE(IntegerType, bool, false, 1);
	STATIC_TYPE(IntegerType, uint8, false, 8);
	STATIC_TYPE(IntegerType, int8, true, 8);
	STATIC_TYPE(IntegerType, uint16, false, 16);
	STATIC_TYPE(IntegerType, int16, true, 16);
	STATIC_TYPE(IntegerType, uint32, false, 32);
	STATIC_TYPE(IntegerType, int32, true, 32);
	STATIC_TYPE(IntegerType, uint64, false, 64);
	STATIC_TYPE(IntegerType, int64, true, 64);
	STATIC_TYPE(IntegerType, uint128, false, 128);
	STATIC_TYPE(IntegerType, int128, true, 128);
	STATIC_TYPE(FloatType, half, 16);
	STATIC_TYPE(FloatType, float, 32);
	STATIC_TYPE(FloatType, double, 64);
	STATIC_TYPE(VoidType, void);

	#undef STATIC_TYPE
	#pragma endregion
}
