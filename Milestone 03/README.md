# Persistence and Registry
Persistence is a crucial aspect of malware behaviour, enabling it to maintain a presence on an infected system over the long term. In the context of Windows operating systems, achieving persistence often involves manipulating the Windows Registry. The malware can make sure that it is launched automatically each time the system boots, before or after user logs in, by making a Registry key entry that points to the location of the executable file.

## Version 1 : Stores and Retrieves Encryption Key
The program creates a registry key named 'MyHive' and stores the encryption key in it. It then calls the different function where it retrieves the encryption key from the registry, recursively traverses a target directory and passes retrieved key as a parameter to the encryption function. This approach avoids  direct usage of the key in the executable and retrieves it at runtime, reducing key exposure. The encryptfile function reads the contents from an input file, encrypts them using the Hill Cipher, and writes the cipher text into the first 4096 bytes of an output file.

## Version 2 : Stores and Retrieves Encryption Key with Boot Up Execution
In this enhanced version of the malware sample (Version 2), additional features have been introduced. The program now includes the capability to add itself to a specific Registry path within the HKCU (HKEY_CURRENT_USER) hive. By doing so, it ensures that the program is automatically executed by the operating system during system startup and when the user logs in. This extended functionality enhances the persistence and impact of the malware, allowing it to maintain a presence on the infected system over an extended period.

### Note:
1. Registry manipulation hives other than HKCU requires administrative privileges.. 
2. To prevent malware sample from executing during boot up, carefully delete the registry entries made by the malware sample, or delete the malware executable. 
3. The malware sample deals with the manipulation of registry entries, kindly handle it with caution. 
4. Revert the registry modifications after the experimentation is completed.

## Version 3 : Hardlinks
Version 3 malware sample has further extended the features of Version 2 sample by using the concept of hard links to maintain persistence. More details can be found within Version 3 folder.

## Setup
To set up the environment for encryption, follow these steps:

1. Create a folder named "newfolder" in the C drive.
2. Inside the "newfolder" directory, create another folder named "capstone". This folder will serve as the target for encryption.
3. You can now create sample text files or subfolders within the "capstone" folder for experimentation purposes.

