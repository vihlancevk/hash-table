# Сделать нормальный Makefile

# out:
# 	g++ -O2 -mavx -mavx2 -c ./src/main.cpp ./src/HashTable.cpp ./src/HashFunctions.cpp ./src/List.cpp ./src/FileOperations.cpp
# 	g++ main.o HashTable.o HashFunctions.o List.o FileOperations.o -o hashTable
# 	./hashTable

out:
	nasm -f elf64 ./src/HashRot13.asm -o HashRot13.o
	g++ -O2 -mavx2 -c ./src/main.cpp ./src/HashTable.cpp ./src/HashFunctions.cpp ./src/List.cpp ./src/FileOperations.cpp
	g++ main.o HashTable.o HashFunctions.o HashRot13.o List.o FileOperations.o -o hashTable
	./hashTable
