__author__ = 'Stefano Carone'

import serial
import time
import threading

#ardu=serial.Serial('/dev/ttyACM0', 9600)
alarm = False
readSerial = ''

class stepMotor:
    def __init__(self, serialListener):
        self.posNow = 0
        self.serialListener = serialListener

    def bedroom(self):
        if (self.posNow != 0):
            if (self.posNow == 2):
                self.serialListener.write(b'21') #Return in main position
                self.posNow = 1
                time.sleep(2)
                self.serialListener.write(b'10')
            elif (self.posNow == 3):
                self.serialListener.write(b'31') #Return in main position
                time.sleep(1)
                self.posNow = 1
                time.sleep(2)
                self.serialListener.write(b'10')
            elif (self.posNow == 1):
                self.posNow = 1
        else:
            self.posNow = 1
            self.serialListener.write(b'10') #Go to the bedroom

    def hall(self):
        if (self.posNow != 0):
            if (self.posNow == 1):
                self.serialListener.write(b'11') #Return in main position
                self.posNow = 2
                time.sleep(2)
                self.serialListener.write(b'20')
            elif (self.posNow == 3):
                self.serialListener.write(b'31') #Return in main position
                self.posNow = 2
                time.sleep(2)
                self.serialListener.write(b'20')
            elif (self.posNow == 2):
                self.posNow = 2
        else:
            self.posNow = 2
            self.serialListener.write(b'20') #Go to the hall room


    def kitchen(self):
        if (self.posNow != 0):
            if (self.posNow == 2):
                self.serialListener.write(b'21') #Return in main position
                self.posNow = 3
                time.sleep(2)
                self.serialListener.write(b'30')
            elif (self.posNow == 1):
                self.serialListener.write(b'11') #Return in main position
                self.posNow = 3
                time.sleep(2)
                self.serialListener.write(b'30')
            elif (self.posNow == 3):
                self.posNow = 3
        else:
            self.posNow = 3
            self.serialListener.write(b'30') #Go to the kitchen room


def rooms(objMotor): #Riconoscimento stanza violata
    global readSerial
    if readSerial == b'001\r\n':
        print('Allarme Camera')
        objMotor.bedroom()
    elif readSerial == b'002\r\n':
        print('Allarme Sala')
        objMotor.hall()
    elif readSerial == b'003\r\n':
        print('Allarme Cucina')
        objMotor.kitchen()
    elif readSerial == b'004\r\n':
        print('Manomissione sistema allarme')

def confAlarm(ser): #Possibilita di attivare/disattivare l'allarme
    while True:
        global alarm
        if alarm == False:
            wantActiveAlarm = input("Vuoi attivare l'allarme(Y/N) --> ")
            if wantActiveAlarm.upper() == 'Y':
                ser.write(b'1')
            else:
                pass
        else:
            password = input('Inserisci passcode: ')
            if ( password == '1234'):
                ser.write(b'1234')
                alarm = False
            else:
                print ('Password is incorrect')

def checkAlarm(ser):
    blockA = False
    blockD = False
    objMotor = stepMotor(ser)
    while True:
        global alarm, readSerial
        readSerial = ser.readline()
        if readSerial == b'101\r\n':
            alarm = True
        elif readSerial == b'100\r\n':
            alarm = False
        if alarm:
            alarm = True
            if not blockA:
                print ('Sistema allarme: ATTIVO')
                ser.write(b's')
                blockA = True
                blockD = False
            rooms(objMotor)
        elif not alarm:
            alarm = False
            if not blockD:
                print ('Sistema allarme: DISATTIVO')
                ser.write(b's')
                blockD = True
                blockA = False
def main():
        try:
            ardu = serial.Serial('COM5', 9600)
            thread_Alarm = threading.Thread(target=confAlarm, name = "Alarm Configuration Thread", args=(ardu,))
            thread_Alarm.start()
            thread_Alarm2 = threading.Thread(target=checkAlarm, name = "Alarm Configuration Thread", args=(ardu,))
            thread_Alarm2.start()
        except KeyboardInterrupt:
            ardu.close()
            print("\n")
            print(__author__)



if __name__ == '__main__':
    main()
