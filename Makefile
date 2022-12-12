CC = g++
CFLAGS = -I -g -w -Wall

main: src/main.cpp src/versionManager/versionManager.cpp src/versionManager/fileSystemVM.cpp
	$(CC) $(CFLAGS) -o main src/main.cpp src/versionManager/versionManager.cpp src/versionManager/fileSystemVM.cpp

clean:
	rm main
	rm /tests/bin/*