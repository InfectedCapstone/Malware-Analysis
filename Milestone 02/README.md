# Ransomware
A simple C program that simulates ransomware in Windows.

## Functionality:
The program encrypts the path "C:\infected" using the Hill Cipher algorithm with a pre-determined key. It then deletes the original files and subfolders, replacing them with new files that have the extension ".enc" added to their names. In addition, the program creates a ransom note in the parent directory, but only if the user executing the program has the necessary write permissions to both the target path and its parent directory.

## Setup

To set up the environment for encryption, follow these steps:

1. Create a folder named "newfolder" in the C drive.
2. Inside the "newfolder" directory, create another folder named "capstone". This folder will serve as the target for encryption.
3. You can now create sample text files or subfolders within the "capstone" folder for experimentation purposes.
