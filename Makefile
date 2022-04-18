out:
	nasm -f elf64 ./src/HashRot13.asm -o HashRot13.o
	g++ -c ./src/main.cpp ./src/HashTable.cpp ./src/HashFunctions.cpp ./src/List.cpp ./src/FileOperations.cpp
	g++ main.o HashTable.o HashFunctions.o HashRot13.o List.o FileOperations.o -o hashTable
	./hashTable
