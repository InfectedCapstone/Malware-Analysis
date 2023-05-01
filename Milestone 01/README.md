# Understanding DLL API calls
  Initially, our focus was to gain an understanding of DLL APIs. To achieve this, we determined that analyzing a malware sample that extensively utilizes various APIs would be the most effective approach. The keylogger malware emerged as an ideal choice for this purpose. 
# Key Logger 
This key logger sample logs key board input stream and writes it into a file.

## Functionality:

  The keylogger is written in C++ specifically for Windows. When executed, it captures and logs the characters typed on the active Command Prompt window into a log file. It does not record keyboard input when other windows are active while the keylogger is running. The keylogging process stops when the user presses CTRL+C. Windows hooks are utilized to attach the keylogger to the keyboard buffer stream, allowing it to log the input stream letter by letter.
