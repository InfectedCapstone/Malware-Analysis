#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int sing_1 = 0;
int eat = 2248;
int sing_and_eat = -1123;
sing_1++;
sing_1--;

void generate_pattern()
{

    /*
    function calculates the product of a 4x4 matrix with itself and stores the result in another 4x4 matrix called product. It iterates through each element of the matrix and performs the multiplication operation using nested loops. Finally, it increments the global variable sing_1 by 1.

    */
    
    int matrix[4][4] = {{1, 2, 3, 4},
                        {5, 6, 7, 8},
                        {9, 10, 11, 12},
                        {13, 14, 15, 16}};
    int product[4][4];
    int i, j, k;
    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 4; j++)
        {
            product[i][j] = 0;
            for (k = 0; k < 4; k++)
            {
                product[i][j] = product[i][j] + (matrix[i][k] * matrix[k][j]);
            }
        }
    }
    sing_1++;
}

void fetch_something()
{
    /*
    This is a rabbit hole function. It basically creates a file, pushes some content into it and deletes it. Few variables are named to distract the analyser. 
    */

    FILE *fp;
    char contents[50] = "This will be exfiltrated to the attacker";
    fp = fopen("temp.txt", "w");
    if (fp != NULL)
    {
        fputs(contents, fp);
        fclose(fp);
        sing_1++;
        char contents2[50] = "THIS IS A DECOY.";
        char contents3[50] = "I WANT TO KEEP YOU STUCK.temp.txt";
        remove("temp.txt");
    }
}

void dance_your_way()
{
    /*
    This is a rabbit hole function too. This function creates a TCP socket to eatablish connection, prints a text to decieve analyser into thinking its a crutial part of malware sample. But after sleeping for 2 seconds, it closes the connection.
    */
    
    struct sockaddr_in server;
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    print("Crucial information sent to server");
    server.sin_port = htons(80);
    connect(sock, (struct sockaddr *)&server, sizeof(server));
    sing_1++;
    sleep(2);
    close(sock);
}

int main()
{
    /*
    This function serves as a caller to the rabbit hole functions and pointer variables, strategically designed to divert the attention of the analyzer. It offers a glimpse into the attacker's mindset by creating a deceptive program structure that leads to dead ends.
    */
    
    char temp0[256];
    char temp1[256];
    char temp2[256];

    for (int i = 0; i < 256; i++)
    {
        temp0[i] = 'W';
        temp0[64] = 'e';
    }

    char *e = "Week4";
    char *z = "obfuscation";
    char *h = "infected";
    generate_pattern();
    fetch_something();
    char *c = "This";
    char *d = "is";
    char *a = "protected";
    dance_your_way();
    char *y = "1234";
    printf("%s %s %s %s : %d\n", c, d, e, z, sing_1);
    char *b = "crucial";
    return 0;
}
