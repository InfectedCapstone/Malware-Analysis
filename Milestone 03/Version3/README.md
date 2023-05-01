# Persistence and Registry with hard links
In Windows, a “hard link” is a file system object that points to the same physical file on disk as another file. It allows multiple file names to refer to the same file, and any changes made to the file are instantly visible to all the file names that reference it.
This version consists of two C files. The first file focuses solely on encryption tasks, which include storing and retrieving encryption keys in the registry. The second file handles the persistence functionality. It adds the paths of two copies of the encryptor and itself to the registry path, ensuring that encryption continues even if one copy is deleted.

## Loader program (encryptor_loader.c)
The program adds itself to the Run path in the Windows registry, ensuring that it is executed when the system starts up and the user logs in. It also creates empty executable files with predefined directory paths and establishes hard links between these files and the original executable. The program hides all the copies, and the original executable to prevent the victim from easily detecting them in the file explorer. It randomly selects one of the copies and checks if it exists, then sets the directory path of that copy in the registry's Run subkey. This way, the loader program adds two entries to the Run subkey: its own path and the path of one of the copies. It should be noted that during the first run of the program, no encryption takes place. 

## Encryptor program (encryptor.c)
This program is responsible for encryption and deletion. However, instead of executing this original executable directly, the program creates copies of it to act as replacements and hide its presence. The copies are the ones that are executed by the system, while the original executable remains hidden. This allows the encryption process to take place without the user directly interacting with the original executable.

## Setup
1. Navigate to the folder where the C files are present.
2. Compile both the C files using the following commands:
    ```
    gcc encryptor.c -o encryptor.exe
    gcc encryptor_loader.c -o encryptor_loader.exe
    ```

3. Create a folder named "newfolder" in the C drive.
4. Inside the "newfolder" directory, create another folder named "capstone". This folder will serve as the target for encryption.
5. You can now create sample text files or subfolders within the "capstone" folder for experimentation purposes.
6. Create a folder named 'newdestination' in the C drive.
7. Copy the "encryptor.exe" executable into the "newdestination" folder.

>To run this version, simply double-click the "encryptor_loader.exe" file.

## Note
1. The malware sample deals with the manipulation of registry entries, kindly handle it with caution. 
2. Revert the registry modifications after the experimentation is completed.
