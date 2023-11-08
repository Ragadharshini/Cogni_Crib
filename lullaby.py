
#After uploading integrated code on Arduino IDE, Open VS New terminal, Change to your directory path, Check IP and Port of your PC (Using Commands) , Music File Path and Then Run the code by typing "python lullaby.py" , You will hear music

#ESP32_IP = "10.18.228.127"

import socket
import pygame

# Define the IP address and port for listening
PC_IP = "10.123.35.137"
PC_PORT = 12345

# Initialize pygame
pygame.init()

# Load the music file
music_file = "C:\\Users\\dhars\\OneDrive\\Desktop\\lullabygoodnight.mp3"
pygame.mixer.init()
pygame.mixer.music.load(music_file)

# Create a socket to listen for incoming signals
server_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
server_socket.bind((PC_IP, PC_PORT))

music_playing = False

while True:
    data, addr = server_socket.recvfrom(1024)
    
    if data.decode() == "Sound Detected":
        if not music_playing:
            pygame.mixer.music.play()
            music_playing = True
        else:
            if not pygame.mixer.music.get_busy():
                # If music has finished playing, reset the flag
                music_playing = False