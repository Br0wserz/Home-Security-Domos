__author__ = 'Stefano Carone'

import serial
import time
import os

#ardu=serial.Serial('/dev/ttyACM0', 9600)
ardu=serial.Serial('COM5', 9600)

class stepMotor:
    def __init__(self, serialListener):
        self.posNow = 0
        self.serialListener = serialListener

    def bedroom(self):
        if (self.posNow != 0):
            if (self.posNow == 2):
                self.serialListener.write(b'B') #Return in main position
                self.posNow = 1
                self.serialListener.write(b'c')
            elif (self.posNow == 3):
                self.serialListener.write(b'A') #Return in main position
                self.posNow = 1
                self.serialListener.write(b'c')
            elif (self.posNow == 1):
                self.posNow = 1
        else:
            self.posNow = 1
            self.serialListener.write(b'c')

    def hall(self):
        if (self.posNow != 0):
            if (self.posNow == 1):
                self.serialListener.write(b'C') #Return in main position
                self.posNow = 2
                self.serialListener.write(b'b')
            elif (self.posNow == 3):
                self.serialListener.write(b'A') #Return in main position
                self.posNow = 2
                self.serialListener.write(b'b')
            elif (self.posNow == 2):
                self.posNow = 2
        else:
            self.posNow = 2
            self.serialListener.write(b'b')


    def kitchen(self):
        if (self.posNow != 0):
            if (self.posNow == 2):
                self.serialListener.write(b'B') #Return in main position
                self.posNow = 3
                self.serialListener.write(b'a')
            elif (self.posNow == 1):
                self.serialListener.write(b'C') #Return in main position
                self.posNow = 3
                self.serialListener.write(b'a')
            elif (self.posNow == 3):
                self.posNow = 3
        else:
            self.posNow = 3
            self.serialListener.write(b'a')



def rooms(ser,objMotor):
    while True:
        readSerial = ser.readline()
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

def confAlarm(ser):


def main():
    while True:
        try:
            impulse = ardu.readline()
            objMotor = stepMotor(ardu)
            time.sleep(2)
            print(impulse)
            if impulse == b'0\r\n':
                print ('Sistema allarme: ATTIVO')
                ardu.write(b'h')
                rooms(ardu,objMotor)
            elif impulse == b'1\r\n':
                print ('Sistema allarme: DISATTIVO')
                confAlarm(ardu)
        except KeyboardInterrupt:
            ardu.close()
            print("\n")
            print(__author__)

if __name__ == '__main__':
    main()
