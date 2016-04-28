FAT: FAT.o main.o
	g++ -g -Wall -Wextra -Werror -std=c++0x -o FAT main.o FAT.o

FAT.o: FAT.cpp
	g++ -g -c -Wall -Wextra -Werror -std=c++0x FAT.cpp

main.o: main.cpp
	g++ -g -c -Wall -Wextra -Werror -std=c++0x main.cpp

clean:
	rm -rf FAT DirEntry.o FAT.o main.o
