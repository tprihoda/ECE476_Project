import random
import socket
import time

SERVER = "192.168.1.110"
PORT = 8080
print("Connecting")
client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client.connect((SERVER, PORT))
print("Connected")
while(1):
        time.sleep(random.expovariate(5))
        client.sendall(bytes("{\"CMD\":1}", 'UTF-8'))
        print(client.recv(128))

