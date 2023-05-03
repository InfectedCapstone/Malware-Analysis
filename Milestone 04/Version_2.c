#include <windows.h>
#include <stdio.h>
#include <string.h>

char MyValue[] = "!@#$+^&*()";

char* foo() 
{

    /*
    This function generates an obfuscated string by manipulating and concatenating various string literals. The resulting obfuscated string is then stored in a dynamically allocated memory and returned as the function's output.
    */
    
    char s1[] = "Stri";
    char s2[] = "ngMa";
    char s3[] = "nipu";
    char s4[] = "lati";
    char s5[] = "on";
    char original_str[64];
    strcpy(original_str, s1);
    strcat(original_str, s2);
    strcat(original_str, s3);
    strcat(original_str, s4);
    strcat(original_str, s5);
    char obfuscated_str[sizeof(original_str)];
    int k = 0;
    for (int i = 0; i < 2; i++) {
        for (int j = i; j < sizeof(original_str); j += 2) {
            obfuscated_str[k++] = original_str[j];
        }
    }
    char obfuscated_str_twisted[sizeof(obfuscated_str) * 2];
    k = 0;
    for (int i = 0; i < sizeof(obfuscated_str); i++) {
        obfuscated_str_twisted[k++] = obfuscated_str[i];
        if (i % 2 == 0 && i < sizeof(obfuscated_str) - 1) {
            obfuscated_str_twisted[k++] = MyValue[i % sizeof(MyValue)];
        }
    }
    char* return_value = (char*)malloc(sizeof(obfuscated_str_twisted));
    strcpy(return_value, obfuscated_str_twisted);
    return return_value;
}

int main() {

    /*
    The obfuscated value obtained from foo() is XORed with 0xFF to to add an extralayer of obfuscation. It creates a registry key under HKEY_CURRENT_USER with the specified subkey, options, and access permissions using RegCreateKeyEx() function. Prints appropriate messages based on the success or failure of creating the key and setting the value. Frees the dynamically allocated memory for the obfuscated value.

    */
    
    char* obfuscated_value = foo();
    char s1[] = "RegCreate";
    char s2[] = "KeyExA";
    char func_name[16];
    strcpy(func_name, s1);
    strcat(func_name, s2);
    HMODULE kernel32_handle = GetModuleHandle("advapi32.dll");
    FARPROC reg_create_key_ex_addr = GetProcAddress(kernel32_handle, func_name);
    typedef LONG(WINAPI* RegCreateKeyExPtr)(HKEY, LPCSTR, DWORD, LPSTR, DWORD, REGSAM, LPSECURITY_ATTRIBUTES, PHKEY, LPDWORD);
    RegCreateKeyExPtr reg_create_key_ex_ptr = (RegCreateKeyExPtr)reg_create_key_ex_addr;
    int len = strlen(obfuscated_value);
    for (int i = 0; i < len; i++) {
        obfuscated_value[i] ^= 0xFF;
    }
    HKEY hKey;
    DWORD dwDisposition;
    LPCSTR lpSubKey = "SOFTWARE\\MyApp";
    DWORD ulOptions = REG_OPTION_NON_VOLATILE;
    REGSAM samDesired = KEY_ALL_ACCESS;
    LPSECURITY_ATTRIBUTES lpSecurityAttributes = NULL;
    PHKEY phkResult = &hKey;
    DWORD dwResult = reg_create_key_ex_ptr(HKEY_CURRENT_USER, lpSubKey, 0, NULL, ulOptions, samDesired, lpSecurityAttributes, phkResult, &dwDisposition);
    if (dwResult == ERROR_SUCCESS) {
        printf("Key created successfully.\n");
        LPCSTR lpValueName = "MyValue";
        DWORD dwType = REG_SZ;
        const BYTE* lpData = (const BYTE*)obfuscated_value;
        DWORD dwDataSize = strlen(obfuscated_value) + 1;
        dwResult = RegSetValueEx(hKey, lpValueName, 0, dwType, lpData, dwDataSize);
        if (dwResult == ERROR_SUCCESS) {
            printf("Value set successfully.\n");}
        else {        printf("Failed to set value. Error code: %d\n", dwResult);    }
    free(obfuscated_value);
    return 0;
    }
}