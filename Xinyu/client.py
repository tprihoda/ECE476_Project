import socket
#create a socket object
num_socket = 4
for i in range (num_socket):
    client = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
#	hostname = socket.gethostname()
#	host =socket.gethostbyname(hostname)
    host = '192.168.1.110'
#define a port on which to connect
#	port = 50000+i
    port = 8080+i
#connect to server on local computer
    client.connect((host,port))
    client.sendall(bytes("Hello", 'UTF-8'))
#receive data from the server
    data = client.recv(1024)
    s = data.decode()
    print(s)
    check = (s=='Hello')
    print(check)
#	data = client.recv(1024)
#	s = data.decode()
#	print(s)
	

	
