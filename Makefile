UNAME := $(shell uname)

CFLAGS = -IEigen -Isource -O2 -pthread -std=c++17 source/*.cpp

ifeq ($(UNAME), Linux)
CFLAGS += -march=native
endif

.PHONY: test
test:
	g++ $(CFLAGS) main.cpp -o test
.PHONY: measure
measure:
	g++ $(CFLAGS) measure.cpp -o measure
.PHONY: gg_recluster
gg_recluster:
	g++ $(CFLAGS) gg_recluster.cpp -o gg_recluster
.PHONY: cut_by_volume 
cut_by_volume:
	g++ $(CFLAGS) cut_by_volume.cpp -o cut_by_volume
.PHONY: cut_by_ratio
cut_by_ratio:
	g++ $(CFLAGS) cut_by_ratio.cpp -o cut_by_ratio
.PHONY: cut_by_treshold
cut_by_treshold:
	g++ $(CFLAGS) cut_by_treshold.cpp -o cut_by_treshold
.PHONY: build_mst
build_mst:
	g++ $(CFLAGS) build_mst.cpp -o build_mst
.PHONY: pipeline
pipeline:
	g++ $(CFLAGS) pipeline.cpp -o pipeline
format:
	clang-format -i *.h *.cpp
