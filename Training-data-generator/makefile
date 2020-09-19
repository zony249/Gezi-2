exe: main.o filters.o images.o counting.o
	g++ main.o filters.o images.o counting.o -o exe -O2 -L/usr/X11R6/lib -lm -lpthread -lX11 -lpng -ljpeg

main.o: src/main.cpp
	g++ -c src/main.cpp -o main.o

counting.o: src/counting.cpp src/counting.hpp
	g++ -c src/counting.cpp -o counting.o

images.o: src/images.cpp src/images.hpp
	g++ -c src/images.cpp -o images.o


filters.o: src/filters.hpp src/filters.cpp
	g++ -c src/filters.cpp -o filters.o
clean:
	rm *.o exe 
