from socket import *
import sys
import time
import PySimpleGUI as sg 


host = sys.argv[1]
port = 5000

client = socket(AF_INET, SOCK_STREAM)

client.connect((host, port))

""" line = client.recv(1024)

client.sendall('valve'.encode())

print(line)  """

#TODO: Recieve pressure data and format it. Save it and display it
#      Send "valve" when open/close clicked

sg.theme('DarkBlue')
c1 = 0.0
c2 = 0.0 
c3 = 0.0

valve = True

layout = [ [sg.Text('P0: ', size=(20, 2) ), sg.Text(key='P0', size=(20, 2))],
            [sg.Text('P1: ', size=(20, 2) ), sg.Text(key='P1', size=(20, 2))],
            [sg.Text('P2: ', size=(20, 2) ), sg.Text(key='P2', size=(20, 2))],
            [sg.Text('Valve state: ', size=(20, 2) ), sg.Text(key='valve', size=(20, 2))],
            [sg.Button('Open 5s', 'left'), sg.Button('Open 1s', 'center'), sg.Button('Open 10s', 'right')],
            [sg.Button('Open'), sg.Button('Close', 'right')],
            [sg.Quit()]]

window = sg.Window('LRE', layout)

while True:
    event, values = window.read(timeout=10)
    if event in ('Quit',sg.WIN_CLOSED):
        break
    window['P0'].update('{0:.2f}'.format(c1))
    window['P1'].update('{0:.2f}'.format(c2))
    window['P2'].update('{0:.2f}'.format(c3))
    window['valve'].update(valve)

    c1 += 1
    c2 += 1
    c3 += 1


window.close()