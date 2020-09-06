from socket import *
import sys
import time
import threading


class server:

    server = socket(AF_INET, SOCK_STREAM)

    h_name = '127.0.0.1'
    port = 5000

    clients = []

    def run(self):
        self.server.bind(('', self.port))
        self.server.listen()
        thread = threading.Thread(target = self.thread_accept, args = [])
        thread.start()

    def thread_accept(self):
        while True:
            con, addr = self.server.accept()
            con.settimeout(0.1)
            self.clients.append(con)

    def send(self, message):
        for x in self.clients:
            try:
                x.send(message.encode())
            except:
                self.clients.remove(x)
    
    def get(self):
        #TODO: Return message + some indentity to verify
        for x in self.clients:
            try:
                mesg = x.recv(1024).decode()
            except:
                
            if len(mesg) > 0:
                return mesg 
            
