.PHONY: test
test:
	g++ *.cpp -std=c++17 -pthread -march=native -o test
format:
	clang-format -i *.h *.cpp
