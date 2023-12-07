all: main workloadgenerator

main: main.cpp zonemaps.cpp 
	g++ -g -std=c++11 -o $@ $^

workloadgenerator: workload_generator.cpp
	g++ -g -std=c++0x -o $@ $^

clean: 
	rm main
	rm workloadgenerator