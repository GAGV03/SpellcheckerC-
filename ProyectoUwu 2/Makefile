CC= g++ -g
all: spellchecker

spellchecker: spellchecker.o
	$(CC) spellchecker.o -o spell

spellchecker.o: spellchecker.cpp 
	$(CC) -c spellchecker.cpp
	
clean:
	rm -f spellchecker spellchecker.o