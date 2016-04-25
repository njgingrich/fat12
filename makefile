FAT: FAT.o main.o
	g++ -g -Wall -Wextra -Werror -std=c++0x -o FAT main.o FAT.o

clean:
	rm -rf FAT FAT.o main.o
