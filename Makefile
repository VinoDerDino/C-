all:
	gcc -o doom_renderer src/main.c -IE:\programming\SDL\x86_64-w64-mingw32\include -LE:\programming\SDL\x86_64-w64-mingw32\lib -lmingw32 -lSDL2main -lSDL2