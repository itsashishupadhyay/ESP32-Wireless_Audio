import serial
import time
import keyboard
import datetime,time

filename="AudioTestAlpha.raw" # If sending Wave data with header rename it to XYZ.wav
datafile=open(filename, 'ab')

print("Start")
port="COM4" #This will be different for various devices and on windows it will probably be a COM port.
bluetooth=serial.Serial(port, 115200,timeout=1)#Start communications with the bluetooth unit
print("Connected")
bluetooth.flushInput() #This gives the bluetooth a little kick

bluetooth.write(b"START  ") #check The ESP Serial Monitor (Space Added after stop using Trial and Error)

while (True):   
    # Time= datetime.utcnow().strftime('%Y-%m-%d %H:%M:%S.%f')[:-3]
    # print(Time)
    input_data=bluetooth.readlines()#This reads the incoming data. In this particular example it will be the "Hello from Blue" line
    # print("test")
    # w.write(bluetooth.readlines())
    # if not input_data:
    #     print("test2")
    #     break
    print(input_data)
    # print(type(input_data))
    # b''.join(input_data)
    # print(input_data)
    # print(type())
    # print(type(input_data))
    # print(type(bytesarray(input_data)))
    datafile.write(b''.join(input_data))
    # # print(input_data.decode())#These are bytes coming in so a decode is needed
    # # time.sleep(0.1) #A pause between bursts
    if keyboard.read_key() == "q":
        print("You pressed Quit")
        # Time= datetime.utcnow().strftime('%Y-%m-%d %H:%M:%S.%f')[:-3]
        # print(Time) 
        break
    
datafile.close()
bluetooth.write(b"STOP  ") #check The ESP Serial Monitor (Space Added after stop using Trial and Error)
print("Done")
bluetooth.close() 