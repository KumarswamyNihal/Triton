from socket import *
import sys
import time

host = sys.argv[1]
port = 5000

client = socket(AF_INET, SOCK_STREAM)

client.connect((host, port))

line = client.recv(1024)

client.sendall('nihal'.encode())

print(line)
