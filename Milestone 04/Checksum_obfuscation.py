import hashlib
import ctypes

# Define the obfuscated API strings
api_str1 = b'@hello'
api_str2 = b'$hi'

# Define the API call signatures
kernel32 = ctypes.WinDLL('kernel32')
FindFirstFile = kernel32.FindFirstFileW
FindFirstFile.restype = ctypes.c_void_p
FindFirstFile.argtypes = [ctypes.c_void_p, ctypes.c_void_p]

FindNextFile = kernel32.FindNextFileW
FindNextFile.restype = ctypes.c_bool
FindNextFile.argtypes = [ctypes.c_void_p, ctypes.c_void_p]

# Calculate the checksums of the obfuscated API strings
api_str1_checksum = hashlib.sha256(api_str1).hexdigest()
api_str2_checksum = hashlib.sha256(api_str2).hexdigest()

# Define the checksums of the desired API strings
desired_api_str1_checksum = 'eb6ed14f1c849cd0d1e11bfbc30892f3c3f09e472ab89af05ed23ca8bfe2b7d1'
desired_api_str2_checksum = 'cd9d881b81a92493130fc732b541a57bdf356c07d96dcb215f305a61d4818db6'

# Load the API calls into memory if the checksums match
if api_str1_checksum == desired_api_str1_checksum:
    FindFirstFile_ptr = ctypes.cast(api_str1, ctypes.c_void_p).value
    FindFirstFile = ctypes.cast(FindFirstFile_ptr, ctypes.CFUNCTYPE(ctypes.c_void_p, ctypes.c_void_p, ctypes.c_void_p))

if api_str2_checksum == desired_api_str2_checksum:
    FindNextFile_ptr = ctypes.cast(api_str2, ctypes.c_void_p).value
    FindNextFile = ctypes.cast(FindNextFile_ptr, ctypes.CFUNCTYPE(ctypes.c_bool, ctypes.c_void_p, ctypes.c_void_p))

# Use the API calls to read the contents of a file
file_handle = FindFirstFile("C:\\infected", None)
while True:
    file_info = ctypes.create_string_buffer(1024)
    if FindNextFile(file_handle, ctypes.byref(file_info)):
        print(file_info.raw.decode('utf-16-le').rstrip('\0'))
    else:
        break

FindClose = kernel32.FindClose
FindClose.restype = ctypes.c_bool
FindClose.argtypes = [ctypes.c_void_p]
kernel32.FindClose(file_handle)
