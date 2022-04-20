.PHONY: test
test:
	g++ *.cpp -std=c++11 -o test
format:
	clang-format -i *.h *.cpp
