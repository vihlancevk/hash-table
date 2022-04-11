out:
	g++ -c ./src/main.cpp ./src/HashTable.cpp ./src/List.cpp
	g++ main.o -o hashTable
	./hashTable
