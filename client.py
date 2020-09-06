from socket import *
import sys
import time
import threading
import PySimpleGUI as sg 


host = ''
#host = sys.argv[1]
port = 5000

client = socket(AF_INET, SOCK_STREAM)
 


#TODO: Recieve pressure data and format it. Save it and display it
#      Send "valve" when open/close clicked


current = time.localtime()
month = current.tm_mon 
hour = current.tm_hour
minute = current.tm_min
sec = current.tm_sec
day = current.tm_mday

filename = str(month)+'.'+str(day)+' '+str(hour)+':'+str(minute)+':'+str(sec) 

f = open(filename, 'w')

sg.theme('DarkBlue')

layout = [[sg.Text('Server IP')], [sg.InputText()], [sg.Button('Open') ,sg.Quit()]]
window = sg.Window('LRE', layout)

while True:
    event, values = window.read(timeout=10)
    if event in ('Quit', sg.WIN_CLOSED):
        break

    if event == 'Open':
        if values[0]:
            host = values[0]
            print(host)
            break

client.connect((host, port))

c0 = 0.0
c1 = 0.0 
c2 = 0.0

valve = False

layout = [ [sg.Text('P0: ', size=(20, 2) ), sg.Text(key='P0', size=(20, 2))],
            [sg.Text('P1: ', size=(20, 2) ), sg.Text(key='P1', size=(20, 2))],
            [sg.Text('P2: ', size=(20, 2) ), sg.Text(key='P2', size=(20, 2))],
            [sg.Text('Valve state: ', size=(20, 2) ), sg.Text(key='valve', size=(20, 2))],
            [sg.Button('Open 5s'), sg.Button('Open 1s'), sg.Button('Open 10s')],
            [sg.Button('Open'), sg.Button('Close')],
            [sg.Quit()]]

window = sg.Window('LRE', layout)

while True:
    event, values = window.read(timeout=10)

    if event in ('Quit',sg.WIN_CLOSED):
        break

    if event == 'Open' and valve == False:
        client.sendall('valve'.encode())

    if event == 'Close' and valve == True:
        client.sendall('valve'.encode())
    
    if event == 'Open 5s':
        client.sendall('valve'.encode())
        thread = threading.Thread(target = close_after, args = (5, ))
        thread.start()
    
    if event == 'Open 1s':
        client.sendall('valve'.encode())
        thread = threading.Thread(target = close_after, args = (1, ))
        thread.start()

    if event == 'Open 10s':
        client.sendall('valve'.encode())
        thread = threading.Thread(target = close_after, args = (10, ))
        thread.start()
    
    line = client.recv(1024).decode()
    f.write(line)
    
    line = line.split(',')

    if len(line) == 5:
        c0 = (((float(line[1])/4096.0)*3.3)*55.5566) - 16.6667
        c1 = (((float(line[2])/4096.0)*3.3)*55.5566) - 16.6667
        c2 = (((float(line[3])/4096.0)*3.3)*55.5566) - 16.6667
        if line[4].rstrip()[1] == '0':
            valve = False
        elif line[4].rstrip()[1] == '1':
            valve = True
        
        
    window['P0'].update('{0:.2f}'.format(c0))
    window['P1'].update('{0:.2f}'.format(c1))
    window['P2'].update('{0:.2f}'.format(c2))
    window['valve'].update(valve)



window.close() 

def close_after(time):
    time.sleep(time)
    client.sendall('valve'.encode())
