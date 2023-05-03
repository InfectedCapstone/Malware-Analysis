#include <string.h>
#include <stdlib.h>

const char* decrypt_string(const char* encrypted_string)
{
    
    /*
    Perform decryption of encrypted_string here. For this sample, we'll just reverse the string

    :param encrypted_string: this string is passed to the function from main.c . 
    */
    
    size_t length = strlen(encrypted_string);
    char* decrypted_string = malloc(length + 1);
    for (size_t i = 0; i < length; i++)
    {
        decrypted_string[i] = encrypted_string[length - i - 1];
    }
    decrypted_string[length] = '\0';
    
    return decrypted_string;
}
