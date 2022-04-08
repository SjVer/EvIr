#ifndef EVIIR__COMMON_H
#define EVIIR__COMMON_H

#if defined(_WIN32) || defined(_WIN64)
#	define API __declspec(dllexport)
#else
#	define API
#endif

#include ".base_pch.hpp"

#pragma region types
namespace eviir
{

typedef std::string 		string;
typedef std::stringstream 	sstream;

}
#pragma endregion

#endif // EVIIR__COMMON_H