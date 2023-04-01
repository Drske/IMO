CXX = clang++
CPPFLAGS = -Wall -g -O3 -std=c++11

main: src/*/*.cpp src/main.cpp src/*/*/*.cpp
	${CXX} ${CPPFLAGS} src/*/*.cpp src/main.cpp src/*/*/*.cpp -o main.out

run : main
	./run.sh
