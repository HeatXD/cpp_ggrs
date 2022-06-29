app: bin/main.o bin/ggrs.o bin/cxx.o
	g++ bin/main.o bin/ggrs.o bin/cxx.o -o app -L. -lcpp_ggrs

bin/main.o: main.cpp 
	g++ -c main.cpp -o bin/main.o

bin/ggrs.o: ../out/cpp_ggrs.cc 
	g++ -c ../out/cpp_ggrs.cc -o bin/ggrs.o

bin/cxx.o: ../out/rust/cxx.cc 
	g++ -c ../out/rust/cxx.cc -o bin/cxx.o