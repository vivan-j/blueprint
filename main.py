import serial
import pygame
import time


PORT = "COM7"
BAUD = 9600
V_SOUND_FILE = "fahhhhhhhhhhhhhh.mp3"
L_SOUND_FILE = "metal-pipe-clang.mp3"

pygame.mixer.init()
sound1 = pygame.mixer.Sound(L_SOUND_FILE)
sound2 = pygame.mixer.Sound(V_SOUND_FILE)

ser = serial.Serial(PORT, BAUD, timeout=1)
print("Listening on " + PORT)

while True:
    try:
        line = ser.readline().decode('utf-8').strip()
        if line:
            print("Received: " + line)

        if "WRONG PIN" in line:
            print("bro you really thought you could get in with the wrong pin :skull:")
            sound1.play()

        elif "UNLOCKED" in line or "LOCKED" in line or "CLEARED" in line:
            sound1.stop()
            if "UNLOCKED" in line:
                sound2.play()

    except Exception as e:
        print("Error: " + str(e))
        time.sleep(1)
