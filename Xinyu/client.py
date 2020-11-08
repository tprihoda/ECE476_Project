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
    host = "10.0.0.10"
#define a port on which to connect

#		port = 8080+i
#connect to server on local computer
    client.connect((host,port))
    print(str(port-8080) +": Connected" + "\n")
    counter = 0
    time.sleep(.5)
    timer = time.perf_counter()
    while(1):
#sample poisson random time
        lamda += .1
        t = random.expovariate(lamda)
        time.sleep(t)
#receive data from the server
        data = "0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789"
        counter += len(data)
        tosend = data.encode()
        client.sendall(tosend)
        if time.perf_counter() - timer > 1:
            print(str(port-8080) +" Bytes: " + str(counter) + "\n")
            counter = 0
            timer = time.perf_counter()
            


lamda = 1
threading1 = threading.Thread(target = client,args= (8080,lamda))
#threading2 = threading.Thread(target = client,args= (8081,lamda))
#threading3 = threading.Thread(target = client,args= (8082,lamda))
#threading4 = threading.Thread(target = client,args= (8083,lamda))
threading1.start()
#threading2.start()
#threading3.start()
#threading4.start()


