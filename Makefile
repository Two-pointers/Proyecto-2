CC = g++ -std=c++2a
CFLAGS = -I -g -w -Wall

main: src/main.cpp src/versionManager/versionManager.cpp src/diff/diff.cpp src/repl/private_graph.cpp src/repl/graph.cpp src/repl/graph.hpp  src/repl/repl.hpp
	$(CC) $(CFLAGS) -o main src/main.cpp src/versionManager/versionManager.cpp src/diff/diff.cpp src/repl/private_graph.cpp src/repl/graph.cpp

clean:
	rm main
	rm /tests/bin/*