	pageCache.o: pageCache.h pageCache.cpp common.h
		gcc -c pageCache.cpp
	centralCache.o: centralCache.h centralCache.cpp pageCache.h pageCache.cpp common.h
		gcc -c centralCache.cpp
	threadCache.o: threadCache.h threadCache.cpp centralCache.h centralCache.cpp pageCache.h pageCache.cpp common.h
		gcc -c pageCache.cpp
	main.o: main.cpp threadCache.h threadCache.cpp centralCache.h centralCache.cpp pageCache.h pageCache.cpp common.h
		gcc -c main.cpp
	test: main.o threadCache.o centralCache.o pageCache.o
		gcc -o test main.o threadCache.o cenralCache.o pageCache.o -lstdc++
