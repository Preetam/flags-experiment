build:
	$(CXX) main.cc -o main -std=c++11

run:
	./main

clean:
	rm main

all: build run

.PHONY: run clean run
