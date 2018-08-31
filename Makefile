main:
	g++ -o core/engine core/main.cpp core/Suffix-Tree.hpp core/tools.hpp -DENABLE_OPENSSL=0 -DENABLE_COMPRESSION=0 -std=c++11 -O3
exec:
	./core/engine