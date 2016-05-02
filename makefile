FAT: FAT.o DirEntry.o main.o
	g++ -g -Wall -Wextra -Werror -std=c++0x -o FAT FAT.o DirEntry.o main.o

FAT.o: FAT.cpp
	g++ -g -c -Wall -Wextra -Werror -std=c++0x FAT.cpp

main.o: main.cpp
	g++ -g -c -Wall -Wextra -Werror -std=c++0x main.cpp

DirEntry.o: DirEntry.cpp
	g++ -g -c -Wall -Wextra -Werror -std=c++0x DirEntry.cpp

clean:
	rm -rf FAT DirEntry.o FAT.o main.o
