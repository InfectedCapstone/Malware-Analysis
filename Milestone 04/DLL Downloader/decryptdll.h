#ifndef DECRYPT_DLL_H
#define DECRYPT_DLL_H

#ifdef BUILD_DLL
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT __declspec(dllimport)
#endif

DLL_EXPORT const char* decrypt_string(const char* encrypted_string);

#endif
