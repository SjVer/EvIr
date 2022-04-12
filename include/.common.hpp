#ifndef EVIR__COMMON_H
#define EVIR__COMMON_H

#if defined(_WIN32) || defined(_WIN64)
#define API __declspec(dllexport)
#else
#define API
#endif

#include ".base_pch.hpp"
#include "tools.hpp"

#pragma endregion
#pragma region types
namespace evir
{

typedef int64_t				int64;
typedef double				float2;
typedef std::string 		String;
typedef std::stringstream 	SStream;

template<typename T>
using Vector = std::vector<T>;

template<typename T1, typename T2>
using Pair = std::pair<T1, T2>;

template<typename K, typename V>
using Map = std::map<K, V>;

}
#pragma endregion

#pragma endregion
#pragma region macros

#define endl ((char)'\n')
#define tab ((char)'\t')

#define STRINGIFY(value) #value
#define COMMA ,

#define ASSERT(condition, whatwentwrong) if(!(condition)) { \
	std::cerr << "EvIr Assertion failed: " << whatwentwrong << endl; raise(SIGABRT); }
#define ASSERT_F(condition, whatwentwrong, ...) ASSERT(condition, tools::fstr(whatwentwrong, __VA_ARGS__))

#pragma endregion

#pragma endregion
#pragma region debug macros
#ifdef DEBUG

#define __DEBUG_MARKER(file, line) "[debug:" file ":" STRINGIFY(line) "]"
#define DEBUG_MARKER __DEBUG_MARKER(__FILE__, __LINE__)
#define DEBUG_PRINT_LINE() std::cout << tools::fstr(\
	DEBUG_MARKER " line %d passed!",__LINE__) << endl
#define DEBUG_PRINT_VAR(value, formatspec) std::cout << tools::fstr(\
	DEBUG_MARKER " var %s = " #formatspec, #value, value) << endl
#define DEBUG_PRINT_MSG(msg) std::cout << DEBUG_MARKER " " msg << endl;
#define DEBUG_PRINT_F_MSG(format, ...) std::cout <<  tools::fstr( \
	DEBUG_MARKER " " format, __VA_ARGS__) << endl

#else

#define DEBUG_MARKER {}
#define DEBUG_PRINT_LINE() {}
#define DEBUG_PRINT_VAR(value, formatspec) {}
#define DEBUG_PRINT_MSG(msg) {}
#define DEBUG_PRINT_F_MSG(format, ...) {}

#endif
#pragma endregion

#endif // EVIR__COMMON_H