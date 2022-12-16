CC = g++ 
CFLAGS = -I -g -w -Wall -std=gnu++2a -lstdc++fs 

main: src/main.cpp src/utils/coloring/coloring.cpp src/utils/types/error.cpp src/versionManager/versionManager.cpp src/diff/diff.cpp src/repl/private_graph.cpp src/repl/graph.cpp  src/repl/repl.hpp src/versionManager/fileSystemVM.cpp src/repl/versionManagement.cpp
	$(CC) $(CFLAGS) -o main src/main.cpp src/utils/coloring/coloring.cpp src/utils/types/error.cpp src/versionManager/versionManager.cpp src/diff/diff.cpp src/repl/private_graph.cpp src/repl/graph.cpp src/versionManager/fileSystemVM.cpp src/repl/versionManagement.cpp

clean:
	rm main
	rm /tests/bin/*