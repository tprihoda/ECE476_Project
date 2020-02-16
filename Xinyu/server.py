#!usr/bin/env python3
import threading
import socket
import numpy as np
from scipy.stats import poisson


hostname = socket.gethostname()# get local machine name
IPAddr = socket.gethostbyname(hostname)#get local ip address
#IPAddr = '192.168.0.10'
port = 8080
#print("Your computer name is: " + hostname)
#print("Your Computer IP Address is: " +IPAddr)
#port = 50000 #reserve a port for the service
list_sock = []
num_sock = 4
#generating poisson distributed data
data_poisson = poisson.rvs(mu = 3, size = 4000)
print(data_poisson[0:10])
for i in range(num_sock):
	s = socket.socket(socket.AF_INET,socket.SOCK_STREAM)#create socket object

	s.setsockopt(socket.SOL_SOCKET,socket.SO_REUSEADDR,1)
	s.bind((IPAddr, port+i))#bind to the port
	s.listen(10)#wait for client connection
	list_sock.append(s)
	print( "[*] Server listening on %s %d" %(IPAddr,(port+i)))
	#establish connection with client
	conn, addr = s.accept()
	print ('Got connection from',addr)
	conn.send('Thank you for connecting'.encode())
	#data = data_poisson[0:10]
	#conn.send(data)
#with conn:
#	print('Connected by',addr)
#	while True:
#		data = conn.recv(1024)
#		if not data: break
#		conn.sendall(data)
