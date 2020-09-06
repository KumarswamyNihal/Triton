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

ser = serial.Serial(port)

current = time.localtime()
month = current.tm_mon 
hour = current.tm_hour
minute = current.tm_min
sec = current.tm_sec
day = current.tm_mday

filename = str(month)+'.'+str(day)+' '+str(hour)+':'+str(minute)+':'+str(sec) 

f = open(filename, 'w')

while True:
    line = ser.readline().decode()
    line = str(time.time())+', '+line
    f.write(line)
