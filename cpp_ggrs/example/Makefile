# windows makefile

app: bin/main.o bin/ggrs.o bin/cxx.o bin/game.o 
	g++ bin/main.o bin/ggrs.o bin/cxx.o bin/game.o -o build/app -O1 -Wall -std=c++17 -Wno-missing-braces -I. -Llib -lcpp_ggrs -lraylib -lopengl32 -lgdi32  -lwinmm 

bin/main.o: src/main.cpp
	g++ -c src/main.cpp -o bin/main.o

bin/ggrs.o: ../out/cpp_ggrs.cc 
	g++ -c ../out/cpp_ggrs.cc -o bin/ggrs.o

bin/cxx.o: ../out/rust/cxx.cc 
	g++ -c ../out/rust/cxx.cc -o bin/cxx.o

bin/game.o: src/game.cpp
	g++ -c src/game.cpp -o bin/game.o

