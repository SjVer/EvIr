#ifndef EVIIR_COMMON_H
#define EVIIR_COMMON_H

#if defined(_WIN32) || defined(_WIN64)
#	define API __declspec(dllexport)
#else
#	define API
#endif

#endif // EVIIR_COMMON_H