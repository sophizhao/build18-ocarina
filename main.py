from machine import Pin, UART
import time
import ustruct

button1 = Pin(2, Pin.IN, Pin.PULL_DOWN)
button2 = Pin(3, Pin.IN, Pin.PULL_DOWN)
button3 = Pin(4, Pin.IN, Pin.PULL_DOWN)
button4 = Pin(5, Pin.IN, Pin.PULL_DOWN)
button5 = Pin(6, Pin.IN, Pin.PULL_DOWN)
button6 = Pin(7, Pin.IN, Pin.PULL_DOWN)
button7 = Pin(8, Pin.IN, Pin.PULL_DOWN)
button8 = Pin(9, Pin.IN, Pin.PULL_DOWN)
button9 = Pin(10, Pin.IN, Pin.PULL_DOWN)
buttons = [button1, button2, button3, button4, button5, button6, button7, button8, button9]

uart = UART(0, baudrate=115200, tx=Pin(4), rx=Pin(5))
uart.init(bits=8, parity=None, stop=2) #??

while True:
    for i, button in enumerate(buttons):
        
        if button.value():
            command = ustruct.pack('B', 0x01 << index)
            uart.write(command)
        else:
            command = ustruct.pack('B', 0x00)
            uart.write(command)
        time.sleep(0.1)