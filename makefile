FAT12: FATDriver.o main.o
	g++ -g -Wall -Wextra -Werror -std=c++0x -o FAT main.o FATDriver.o

clean:
	rm -rf FAT12 FATDriver.o main.o
