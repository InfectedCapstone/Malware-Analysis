# Command and Control (C2):
A Command and Control (C2) infrastructure serves as the centralized communication platform for controlling and managing compromised systems or malware. It enables attackers to remotely issue commands, receive data, and maintain persistence in their malicious activities, often operating covertly to evade detection. C2 plays a critical role in advanced cyber attacks, allowing threat actors to exert control over compromised systems and carry out their objectives. So this milestone includes a client and server code which simulated the C2 action on a simple scale.

## Client side: client.c
This will impersonate victim. It is assumed that victim has the malicious executable which is client.exe. Once this is executed by victim, is establishes TCP connection with server/ attacker. Based on string recieved from server (FindFirstFile in this sample), client sends contents of private_data.txt and closes connection. In a real-world scenario, the connection may remain open, serving as a backdoor for further data exfiltration.

## Server side: server.c
This will act as attack C2. It listens at port 8000 waiting for client to connect. Once connected, sends the data from commands.txt file. Recieves data from client which will be stored in exfiltrated_data.txt. 

### SetUp:
1. Connect two Windows systems or VMs to the same network.
2. Create a file named "private_data.txt" on the client side in the same directory as "client.c".
3. Create a file named "commands.txt" on the server side in the same directory as "server.c".
4. Execute the server-side code to start the server program.
```
gcc server.c -o server.exe -l ws2_32
server.exe
```
5. Execute the client side code.
```
gcc client.c -o client.exe -l ws2_32
client.exe
```

## Data exfiltration in action
1) The server is listening for any incoming client requests. When a client connects to it, the server sends the commands stored in a text file to the client. For simplicity, the text file stores only one command – FindFirstFile 
2) The client just has to run the executable. The client code creates a socket connection to the server. On receiving the command from the server, the client code verifies (at run time) if it is equivalent to “FindFirstFile” 
3) If it is, then it opens a file that contains critical information (we have hard coded some passwords into this text file) and exfiltrates the contents of this to the server. 
4) On receiving these commands from the client, the server creates a text file and loads all the content fetched from the client. 
5) The client closes the connection after the exfiltration, just like a real-world malware sample would, after the interaction 
