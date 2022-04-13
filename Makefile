out:
	g++ -c ./src/main.cpp ./src/HashTable.cpp ./src/HashFunctions.cpp ./src/List.cpp ./src/FileOperations.cpp
	g++ main.o HashTable.o HashFunctions.o List.o FileOperations.o -o hashTable
	./hashTable
