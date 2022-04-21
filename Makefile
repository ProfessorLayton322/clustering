.PHONY: test
test:
	g++ *.cpp -std=c++11 -pthread -o test
format:
	clang-format -i *.h *.cpp
