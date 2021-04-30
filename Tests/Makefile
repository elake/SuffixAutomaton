OBJS	= VectorTiming.o MapTiming.o
SOURCE	= VectorTiming.cpp MapTiming.cpp
OUT	= VectorTiming MapTiming PositionsTest
CC	 = g++
FLAGS	 = -g -c

all: VectorTiming MapTiming PositionsTest

PositionsTest: PositionsTest.o
	g++ -g PositionsTest.o -o PositionsTest

VectorTiming: VectorTiming.o
	g++ -g VectorTiming.o -o VectorTiming

MapTiming: MapTiming.o
	g++ -g MapTiming.o -o MapTiming

PositionsTest.o: PositionsTest.cpp
	$(CC) $(FLAGS) PositionsTest.cpp -std=c++17

VectorTiming.o: VectorTiming.cpp
	$(CC) $(FLAGS) VectorTiming.cpp -std=c++17

MapTiming.o: MapTiming.cpp
	$(CC) $(FLAGS) MapTiming.cpp -std=c++17

test0: PositionsTest
	@printf "This test checks for correctness by using the automaton to find the positions of substrings in texts. The smallest text is one sentence, the largest text is the unabridged Bible (2113 pages in MS Word). The test will commence in 5 seconds."
	sleep 5
	./PositionsTest
	@printf "Reults are saved in positionsresults.csv"

test1: VectorTiming
	@printf "Testing construction times for linearity on English text. Test will commence in 5 seconds."
	sleep 5
	echo bible | ./VectorTiming
	@printf "This test performed a series of suffix automaton constructions on increasingly large segments of an English Bible. The figures you see above represent the size of the string, as well as the time it took to construct. The ratio of input size to construction time will experience some variance, but the goal here is to demonstrate that it stays fairly consistent with regard to input size, which is what we would expect of a linear time O(n) construction. This test uses vectors to store transitions, with linear search for transition lookup. These results are saved to biblevectortiming.csv"

test2: VectorTiming
	echo moststates | ./VectorTiming
	@printf "This test performed a series of suffix automaton constructions on increasingly large strings designed to produce the maximum possible number of states in the automaton. The figures you see above represent the size of the string, as well as the time it took to construct. The ratio of input size to construction time will experience some variance, but the goal here is to demonstrate that it stays fairly consistent with regard to input size, which is what we would expect of a linear time construction. This test uses vectors to store transitions, with linear search for transition lookup. These results are saved to moststatesvectortiming.csv"

test3: VectorTiming
	echo mosttransitions | ./VectorTiming
	@printf "This test performed a series of suffix automaton constructions on increasingly large strings designed to produce the maximum possible number of transitions between states. The figures you see above represent the size of the string, as well as the time it took to construct. The ratio of input size to construction time will experience some variance, but the goal here is to demonstrate that it stays fairly consistent with regard to input size, which is what we would expect of a linear time construction. This test uses vectors to store transitions, with linear search for transition lookup. These results are saved to mosttransitionsvectortiming.csv"

test4: MapTiming
	echo bible | ./MapTiming
	@printf "This test performed a series of suffix automaton constructions on increasingly large segments of an English Bible. The figures you see above represent the size of the string, as well as the time it took to construct. The ratio of input size to construction time will experience some variance, but the goal here is to demonstrate that it stays fairly consistent with regard to input size, which is what we would expect of a linear time O(n) construction. This test uses map to store transitions, to avoid linear search for transition lookup. These results are saved to biblemaptiming.csv"

test5: MapTiming
	echo moststates | ./MapTiming
	@printf "This test performed a series of suffix automaton constructions on increasingly large strings designed to produce the maximum possible number of states in the automaton. The figures you see above represent the size of the string, as well as the time it took to construct. The ratio of input size to construction time will experience some variance, but the goal here is to demonstrate that it stays fairly consistent with regard to input size, which is what we would expect of a linear time construction. This test uses map to store transitions, to avoid linear search for transition lookup. These results are saved to moststatesmaptiming.csv"

test6: MapTiming
	echo mosttransitions | ./MapTiming
	@printf "This test performed a series of suffix automaton constructions on increasingly large strings designed to produce the maximum possible number of transitions between states. The figures you see above represent the size of the string, as well as the time it took to construct. The ratio of input size to construction time will experience some variance, but the goal here is to demonstrate that it stays fairly consistent with regard to input size, which is what we would expect of a linear time construction. This test uses map to store transitions, to avoid linear search for transition lookup. These results are saved to mosttransitionsmaptiming.csv"

clean:
ifeq ($(OS),Windows_NT)
	$(RM) *.exe
	$(RM) *.o
	$(RM) *.csv
else
	$(RM) $(OBJS) $(OUT)
	$(RM) *.csv
endif
	