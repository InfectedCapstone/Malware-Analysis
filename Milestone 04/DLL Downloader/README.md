# DLL Downlaoder:
Downloaders are also usually non-malicious executables that download malicious payloads from C2 servers into the host system. This program downloads three files from a localhost apache server using the URLDownloadToFile function. 

## Our non-malicious file: main.c
It dynamically loads the downloaded DLL using the LoadLibrary function. It retrieves the function pointer for the "decrypt_string" function from the loaded DLL using the GetProcAddress function. It then uses this function to decrypt a string. It retrieves the function pointer for the "FindFirstFileA" function from the kernel32.dll by using the decrypted string obtained in the previous step.Finally, it calls the "FindFirstFileA" function with a filename pattern to list the contents of the "C:\infected" directory. Finally, it frees the loaded decryption DLL using the FreeLibrary function.

## Decryptor: decryptdll.c
The encrypted string passed to this function will be decrypted (reversed in our sample) and returned.

# SetUp:
## Setup Apache server:
### Here are the steps to host an Apache server on Windows:
1. Download and install Apache HTTP server for Windows from the Apache Lounge website (https://www.apachelounge.com/download/).
2. Once downloaded, extract the files to a location of your choice, such as C:\Apache24.
3. Navigate to the Apache24\conf directory and open the httpd.conf file in a text editor.
4. Find the line that reads "Listen 80" and ensure that it is not commented out (i.e., does not have a "#" character at the beginning of the line). If it is commented out, remove the "#" character and save the file.
5. Find the line that reads "ServerName localhost:80" and ensure that it is not commented out. If it is commented out, remove the "#" character and save the file.
6. Open a command prompt and navigate to the Apache24\bin directory.
7. Run the command "httpd.exe -k install" to install the Apache service.
8. Once the service is installed, run the command "httpd.exe" to start the Apache server.
9. Open a web browser and navigate to http://localhost/. You should see the default Apache welcome page if the server is running properly.
10. To host decryptdll.c and .h on your local Apache server, simply place them in the Apache24\htdocs directory and access them via the appropriate URL (e.g., http://localhost/decryptdll.c). 
11. Place main.c file in a different folder.

## Execution Setup:
1. Create a folder named 'infected' in C drive. Create a text file in this folder.
2. Navigate to the folder where main.c is present.
3. Compile the C file. 
```
gcc main.c -o main.exe -lurlmon
```
4. Run the executable.