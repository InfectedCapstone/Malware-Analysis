# OBFUSCATION
Software code can be made more challenging to interpret or reverse engineer by using the obfuscation 
approach. Obfuscation is the deliberate effort to make something confusing or difficult to read, usually with the aim of making it more difficult to reverse engineer, copy, or analyse. Malware writers also utilise it to make their code more challenging for security experts to find and analyse. This milestone incluldes various samples which apply different obfuscation techniques.

## Version 1: String Obfuscation
It involves transforming strings into a different representation, such as using encoding, encryption, or splitting the string into smaller parts. This makes it harder for analysts to directly understand the purpose and content of the obfuscated strings. The sample has same functionality as that of Milestone 03 version 2 sample with added obfuscation features. This sample employs few techniques such as stack splittingn string encoding,etc.

## Version 2: Function name Obfuscation
The method where the author tries to hide the function name and its working from the analyst and 
when executed the function works the same as it is intended to. This program uses string manipulation technique of separating the function name and then concatenating it before calling the 'RegCreatKey' API. This approach makes it difficult for reverse engineering software like Ghidra to detect the API call directly. It's important to note that this technique has been applied to only one API call in this particular code, but it can be extended to other API and function calls as well.

## Version 3: Checksum based Obfuscation
This is a runtime obfuscation technique. It is a technique used to obfuscate data by generating a checksum and using it to mask or encrypt the original data. This sample uses checksum obfuscation. Initially, it define obfuscated API strings and API call signatures from kernel32.dll.Calculate checksums of obfuscated API strings and define desired checksums. If checksums match, load API calls into memory as function pointers. Based on the match, it calls FindFirstFile to search for files in a directory and loop through FindNextFile for file retrieval.

> Note: Static Analysis is harder with python executable than C executable. Because python is a high-level language, which means that it has a large and complex runtime environment that includes many built-in functions and libraries. This can result in a large number of functions being present in the binary, which can make it harder to identify the relevant code.

## Rabbit hole: 
A rabbit hole in malware refers to a deceptive technique used by attackers to mislead and confuse analysts during the analysis process. It involves the inclusion of seemingly important or complex code, functions, or data that serve no real purpose in the malware's functionality. Hence, diverting malware analysists. This sample is a collection of few basic rabbit hole techniques that could be employed by attackers.

## DLL Downloader: 
A sample based on DLL Downloader concept is present in this folder.


