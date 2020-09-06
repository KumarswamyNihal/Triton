import server
import time
import serial.tools.list_ports 
import serial 

Server = server.server()

Server.run()

""" time.sleep(5)
Server.send('TEST')

while True: 
    mesg = Server.get()
    if mesg:
        print(mesg) """

ports = list(serial.tools.list_ports.comports())

port = ''

for x in ports:
    if 'Maple' in x.description:
        port = x.device 

#TODO: Read port and Server.send() (p0, p1, p2, valve)
#      Server.get() If == 'valve' switch valve position