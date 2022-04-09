#ifndef EVIIR__COMMON_H
#define EVIIR__COMMON_H

#if defined(_WIN32) || defined(_WIN64)
#define API __declspec(dllexport)
#else
#define API
#endif

#include ".base_pch.hpp"
#include "tools.hpp"

#pragma region types
namespace eviir
{

typedef std::string 		string;
typedef std::stringstream 	sstream;

template<typename T>
using vector = std::vector<T>;

}
#pragma endregion

#pragma region macros

#define STRINGIFY(value) #value

#define ASSERT(condition, whatwentwrong) assert(condition && whatwentwrong)

#pragma endregion

#pragma region debug macros
#ifdef DEBUG

#define __DEBUG_MARKER(file, line) "[debug:" file ":" STRINGIFY(line) "]"
#define DEBUG_MARKER __DEBUG_MARKER(__FILE__, __LINE__)
#define DEBUG_PRINT_LINE() std::cout << tools::fstr(\
	DEBUG_MARKER " line %d passed!",__LINE__) << std::endl
#define DEBUG_PRINT_VAR(value, formatspec) std::cout << tools::fstr(\
	DEBUG_MARKER " var %s = " #formatspec, #value, value) << std::endl
#define DEBUG_PRINT_MSG(msg) std::cout << DEBUG_MARKER " " msg << std::endl;
#define DEBUG_PRINT_F_MSG(format, ...) std::cout <<  tools::fstr( \
	DEBUG_MARKER " " format, __VA_ARGS__) << std::endl

#else

#define DEBUG_MARKER {}
#define DEBUG_PRINT_LINE() {}
#define DEBUG_PRINT_VAR(value, formatspec) {}
#define DEBUG_PRINT_MSG(msg) {}
#define DEBUG_PRINT_F_MSG(format, ...) {}

#endif
#pragma endregion

#endif // EVIIR__COMMON_H