import socket
#import numpy as np
import time
import random
import threading
#create a socket object
def client(port,lamda):
#	for i in range (num_socket):
    client = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
#hostname = socket.gethostname()
#host =socket.gethostbyname(hostname)
    host = "192.168.1.110"
#define a port on which to connect

#		port = 8080+i
#connect to server on local computer
    client.connect((host,port))
    while(1):
#sample poisson random time
        t = random.expovariate(lamda)
        time.sleep(t)
#receive data from the server
        data = "Hello"
        tosend = data.encode()
        client.sendall(tosend)
        data = client.recv(1024)
        s = data.decode()
        print(s)
#		check = (s=='Thank you for connecting')
#		print(check)



threading1 = threading.Thread(target = client,args= (8080,1))
threading2 = threading.Thread(target = client,args= (8081,2))
threading3 = threading.Thread(target = client,args= (8082,3))
threading4 = threading.Thread(target = client,args= (8083,4))
threading1.start()
threading2.start()
threading3.start()
threading4.start()


