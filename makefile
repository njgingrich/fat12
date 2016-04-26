FAT: FAT.o DirEntry.o main.o
	g++ -g -Wall -Wextra -Werror -std=c++0x -o FAT main.o FAT.o DirEntry.o

clean:
	rm -rf FAT DirEntry.o FAT.o main.o
