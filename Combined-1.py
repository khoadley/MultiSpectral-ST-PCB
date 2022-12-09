#!/usr/bin/python
import numpy as np
import matplotlib.pyplot as plt
from matplotlib import gridspec
from drawnow import drawnow
import tkinter as tk
import serial.tools.list_ports
import serial
import sys
import glob
import time
import csv
from time import sleep

ports = serial.tools.list_ports.comports(include_links=False)
for port in ports :
    print(port.device)
    #print(ports[2])

timeSer = []
Col0Ser = []
Col1Ser = []
Col2Ser = []
Col3Ser = []

Col0xSer = []
Col1xSer = []
Col2xSer = []
Col3xSer = []

widge = 'new'
    
def show_entry_fields():
   ser = serial.Serial(port.device)
   #if ser.isOpen():
   #    ser.close()

   ser = serial.Serial(port.device, 9600, timeout=1)
   ser.flushInput()
   ser.flushOutput()
   print('Connect ' + ser.name)  
   #ser=serial.Serial(ser.name, 9600, writeTimeout = 1)
   #ser=serial.Serial(porter[1], 9600, writeTimeout = 0)  
   #print("Pulse-length(uSec): %s\nIterations: %s\nDelay-between-Iterations(Millisec): %s\nFile-Name: %s\nProgram-Run: %s" % (e1.get(), e2.get(), e3.get(), e4.get(), e5.get(), e6.get()))
   ser.write(e1.get().encode())
   ser.write(",".encode())
   ser.write(e2.get().encode())
   ser.write(",".encode())
   ser.write(e3.get().encode())
   ser.write(",".encode())
   ser.write(e4.get().encode())
   ser.write(",".encode())
   ser.write(e5.get().encode())
   ser.write(",".encode())
   ser.write(e6.get().encode())
   ser.write(",".encode())
   ser.write(e7.get().encode())
   ser.write(",".encode())
   ser.write(e8.get().encode())
   ser.write(",".encode())
   ser.write(e9.get().encode())
   ser.write(",".encode())   
   ser.write("STOP".encode())
   #ser.close()
      
   global fig
   
   time0 = []
   time1 = []
   time2 = []
   time3 = []

   Col0 = []
   Col1 = []
   Col2 = []
   Col3 = []   

   ser = serial.Serial(port.device)
            
   ser.flushInput()
   ser.flushOutput()
   status = 'test'
   
   while 1:
       
      val = ser.readline().decode()
      datstream = val.split()
      status == datstream[0]
      
      #print(len(datstream))
      if len(datstream) == 3:
         
           status = datstream[2]
           #print(datstream[2])
   
           if status == 'CURVES0':
               time0.append(float(datstream[0]))
               Col0.append(float(datstream[1]))
   
           if status == 'CURVES1':
               time1.append(float(datstream[0]))
               Col1.append(float(datstream[1]))
   
           if status == 'CURVES2':
               time2.append(float(datstream[0]))
               Col2.append(float(datstream[1]))
   
           if status == 'CURVES3':
               time3.append(float(datstream[0]))
               Col3.append(float(datstream[1]))
               
           elif status == 'END':
               plt.clf()
               gs = gridspec.GridSpec(3,4, hspace=0.1, wspace=0.1)
               timemax0 = (float(max([float(i) for i in time0]))*1.1)
               timemax1 = (float(max([float(i) for i in time1]))*1.1)
               timemax2 = (float(max([float(i) for i in time2]))*1.1)
               timemax3 = (float(max([float(i) for i in time3]))*1.1)
               
               ax1 = fig.add_subplot(gs[0,0])
               ax1.scatter(time0, Col0, s=5, c='#756bb1', lw=0)
               Col0max = (float(max([float(i) for i in Col0]))*1.25)
               plt.ylim(0,Col0max)
               plt.xlim(0,timemax0)
               plt.title('Col 0 LED', fontsize=5)
               #plt.xlabel('Time (usec)')
               plt.grid(True)
   
               ax2 = fig.add_subplot(gs[0,1])
               ax2.scatter(time1, Col1, s=5, c='#2171b5', lw=0)
               Col1max = (float(max([float(i) for i in Col1]))*1.25)
               plt.ylim((0,Col1max))
               plt.xlim((0,timemax1))
               plt.title('Col 1 LED', fontsize=5)
               #plt.xscale('log')
               #plt.xlabel('Time (usec)')
               plt.grid(True)
   
               ax3 = fig.add_subplot(gs[0,2])
               ax3.scatter(time2, Col2, s=5, c='#4eb3d3', lw=0)
               Col2max = (float(max([float(i) for i in Col2]))*1.25)
               plt.ylim((0, Col2max))
               plt.xlim((0,timemax2))
               plt.title('Col 2 LED', fontsize=5)
               #plt.xlabel('Time (usec)')
               plt.grid(True)
   
               ax4 = fig.add_subplot(gs[0,3])
               ax4.scatter(time3, Col3, s=5, c='#41ae76', lw=0)
               Col3max = (float(max([float(i) for i in Col3]))*1.25)
               plt.ylim((0,Col3max))
               plt.xlim((0,timemax3))
               plt.title('Col 3 LED', fontsize=5)
               plt.xlabel('Time (usec)', fontsize=5)
               plt.grid(True)
               
               time0 = []
               time1 = []
               time2 = []
               time3 = []
               
               Col0 = []
               Col1 = []
               Col2 = []
               Col3 = []
               
      elif len(datstream) == 14:
           status = datstream[13]
           if status == 'PHOTOCHEM':
            
               global timeSer, Col0Ser, Col1Ser, Col2Ser, Col3Ser, Col0xSer, Col1xSer, Col2xSer, Col3xSer
               timeSer.append(float(datstream[12]))
               Col0Ser.append(float(datstream[8]))
               Col1Ser.append(float(datstream[9]))
               Col2Ser.append(float(datstream[10]))
               Col3Ser.append(float(datstream[11]))
   
               Col0xSer.append(float(datstream[4]))
               Col1xSer.append(float(datstream[5]))
               Col2xSer.append(float(datstream[6]))
               Col3xSer.append(float(datstream[7]))
               
               ax5 = fig.add_subplot(gs[1,:])
               ax5.plot(timeSer, Col0Ser, '#756bb1', timeSer, Col1Ser, '#2171b5', timeSer, Col2Ser, '#4eb3d3', timeSer, Col3Ser, '#41ae76')
               plt.ylim((0,0.5))
               plt.ylabel('Fv/Fm', fontsize=5)
               plt.xlabel('Time (sec)', fontsize=5)
               plt.grid(True)
   
               ax6 = fig.add_subplot(gs[2,:])
               ax6.plot(timeSer, Col0xSer, '#756bb1', timeSer, Col1xSer, '#2171b5', timeSer, Col2xSer, '#4eb3d3', timeSer, Col3xSer, '#41ae76')
               plt.ylim((0,4))
               plt.ylabel('Sigma', fontsize=5)
               plt.xlabel('Time (sec)', fontsize=5)
               plt.grid(True)
   
               gs.update(wspace=0.5, hspace=0.5)
               fig = plt.gcf()
               plt.show()
               plt.pause(0.0005)
               
      elif len(datstream) == 2:
           status = datstream[0]
           if status == 'complete':
               ser.close()
   
   else:
       exit()

def deleter():
    
      ser = serial.Serial(port.device)
      
      ser.flushInput()
      ser.flushOutput()
      print('Connect ' + ser.name)  
      ser.write("1".encode())
      ser.write(",".encode())
      ser.write("1".encode())
      ser.write(",".encode())
      ser.write("100".encode())
      ser.write(",".encode())
      ser.write("test".encode())
      ser.write(",".encode())
      ser.write("11".encode())
      ser.write(",".encode())
      ser.write("100".encode())
      ser.write(",".encode())
      ser.write("100".encode())
      ser.write(",".encode())
      ser.write("100".encode())
      ser.write(",".encode())
      ser.write("100".encode())
      ser.write(",".encode())
      ser.write("STOP".encode())
      ser.close()
   
   
def dwnld():
    
      ser = serial.Serial(port.device)
      
      ser.flushInput()
      ser.flushOutput()
      print('Connect ' + ser.name)  
      #ser=serial.Serial(ser.name, 9600, writeTimeout = 1)
      #ser=serial.Serial(porter[1], 9600, writeTimeout = 0)  
      #print("Pulse-length(uSec): %s\nIterations: %s\nDelay-between-Iterations(Millisec): %s\nFile-Name: %s\nProgram-Run: %s" % (e1.get(), e2.get(), e3.get(), e4.get(), e5.get(), e6.get()))
      ser.write("1".encode())
      ser.write(",".encode())
      ser.write("1".encode())
      ser.write(",".encode())
      ser.write("100".encode())
      ser.write(",".encode())
      ser.write("test".encode())
      ser.write(",".encode())
      ser.write("10".encode())
      ser.write(",".encode())
      ser.write("100".encode())
      ser.write(",".encode())
      ser.write("100".encode())
      ser.write(",".encode())
      ser.write("100".encode())
      ser.write(",".encode())
      ser.write("100".encode())
      ser.write(",".encode())
      ser.write("STOP".encode())
      ser.close()
  
      ser = serial.Serial(port.device)
      
      ser.flushInput()
      ser.flushOutput()
      
      status = 'test'
      
      while 1:
          
          val = ser.readline().decode()
          datstream = val.split()
          if len(datstream) == 2:
              status = datstream[0]
              if status == 'dump':
                  ser.close()
              print(status)
              
          else:
              with open(str(status),"a") as f:
                   writer = csv.writer(f,delimiter=" ")
                   writer.writerow(datstream)
                   
                   

def clearing():
   global timeSer, Col0Ser, Col1Ser, Col2Ser, Col3Ser, Col0xSer, Col1xSer, Col2xSer, Col3xSer
              
   timeSer = []
   Col0Ser = []
   Col1Ser = []
   Col2Ser = []
   Col3Ser = []

   Col0xSer = []
   Col1xSer = []
   Col2xSer = []
   Col3xSer = []


master = tk.Tk()
tk.Label(master, text="Light Curve Step Delay (seconds)").grid(row=0)
tk.Label(master, text="Iterations").grid(row=1)
tk.Label(master, text="Delay-between-Iterations(Millisec)").grid(row=2)
tk.Label(master, text="File-Name").grid(row=3)
tk.Label(master, text="Program-Run").grid(row=4)
tk.Label(master, text="Col 0 Pulse-length(uSec)").grid(row=5)
tk.Label(master, text="Col 1 Pulse-length(uSec)").grid(row=6)
tk.Label(master, text="Col 2 Pulse-length(uSec)").grid(row=7)
tk.Label(master, text="Col 3 Pulse-length(uSec)").grid(row=8)


e1 = tk.Entry(master)
e2 = tk.Entry(master)
e3 = tk.Entry(master)
e4 = tk.Entry(master)
e5 = tk.Entry(master)
e6 = tk.Entry(master)
e7 = tk.Entry(master)
e8 = tk.Entry(master)
e9 = tk.Entry(master)

e1.grid(row=0, column=1)
e2.grid(row=1, column=1)
e3.grid(row=2, column=1)
e4.grid(row=3, column=1)
e5.grid(row=4, column=1)
e6.grid(row=5, column=1)
e7.grid(row=6, column=1)
e8.grid(row=7, column=1)
e9.grid(row=8, column=1)


tk.Button(master, text='Quit', command=master.quit).grid(row=9, column=0, sticky=tk.W, pady=4)
tk.Button(master, text='Update', command=show_entry_fields).grid(row=9, column=1, sticky=tk.W, pady=4)
tk.Button(master, text='clear', command=clearing).grid(row=10, column=0, sticky=tk.W, pady=4)
tk.Button(master, text='Download', command=dwnld).grid(row=10, column=1, sticky=tk.W, pady=4)
tk.Button(master, text='delete', command=deleter).grid(row=11, column=1, sticky=tk.W, pady=4)
   
plt.ion()
fig =plt.figure(figsize=(8.5,6.5))   

master.mainloop( )





