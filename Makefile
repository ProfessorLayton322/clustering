.PHONY: test
test:
	g++ test_clustering.cpp generate_tests.cpp prim.cpp prim_thread.cpp point.cpp graph.cpp -o test -std=c++11
format:
	clang-format -i *.h *.cpp
