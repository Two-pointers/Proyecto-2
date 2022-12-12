CC = g++
CFLAGS = -I -g -w -Wall

main: src/main.cpp src/versionManager/versionManager.cpp
	$(CC) $(CFLAGS) -o main src/main.cpp src/versionManager/versionManager.cpp

clean:
	rm main
	rm /tests/bin/*