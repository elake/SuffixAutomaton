OBJS	= VectorTiming.o MapTiming.o
SOURCE	= VectorTiming.cpp MapTiming.cpp
OUT	= VectorTiming MapTiming PositionsTest
CC	 = g++
FLAGS	 = -g -c

all: VectorTiming MapTiming PositionsTest SuffixAutomaton

SuffixAutomaton: SuffixAutomaton.o
	g++ -g SuffixAutomaton.o -o SuffixAutomaton

PositionsTest: PositionsTest.o
	g++ -g PositionsTest.o -o PositionsTest

VectorTiming: VectorTiming.o
	g++ -g VectorTiming.o -o VectorTiming

MapTiming: MapTiming.o
	g++ -g MapTiming.o -o MapTiming

SuffixAutomaton.o: SuffixAutomaton.cpp
	$(CC) $(FLAGS) SuffixAutomaton.cpp -std=c++17

PositionsTest.o: PositionsTest.cpp
	$(CC) $(FLAGS) PositionsTest.cpp -std=c++17

VectorTiming.o: VectorTiming.cpp
	$(CC) $(FLAGS) VectorTiming.cpp -std=c++17

MapTiming.o: MapTiming.cpp
	$(CC) $(FLAGS) MapTiming.cpp -std=c++17

run: SuffixAutomaton
	./SuffixAutomaton

test0: PositionsTest
	@printf "This test checks for correctness by using the automaton to find the positions of substrings in texts. The smallest text is one sentence, the largest text is the unabridged Bible (2113 pages in MS Word).\nThe test will commence in 5 seconds."
	@sleep 5
	./PositionsTest
	@printf "Reults are saved in positionsresults.csv\n"

test1: VectorTiming
	@printf "Testing construction times for linearity on English text.
	@printf "Test will commence in 5 seconds."
	@sleep 5
	echo bible | ./VectorTiming
	@printf "This test performed a series of suffix automaton constructions on increasingly large segments of an English Bible. The figures you see above represent the size of the string, as well as the time it took to construct. The ratio of input size to construction time will experience some variance, but the goal here is to demonstrate that it stays fairly consistent with regard to input size, which is what we would expect of a linear time O(n) construction. This test uses vectors to store transitions, with linear search for transition lookup. These results are saved to biblevectortiming.csv\n"

test2: VectorTiming
	@printf "This test performs a series of suffix automaton constructions on increasingly large strings designed to produce the maximum possible number of states in the automaton.\n"
	@printf "Test will commence in 5 seconds.\n"
	@sleep 5
	echo moststates | ./VectorTiming
	@printf "The figures you see above represent the size of the string, as well as the time it took to construct. The ratio of input size to construction time will experience some variance, but the goal here is to demonstrate that it stays fairly consistent with regard to input size, which is what we would expect of a linear time construction. This test uses vectors to store transitions, with linear search for transition lookup. These results are saved to moststatesvectortiming.csv\n"

test3: VectorTiming
	@printf "This test performs a series of suffix automaton constructions on increasingly large strings designed to produce the maximum possible number of transitions between states.\n"
	@printf "Test will commence in 5 seconds."
	@sleep 5
	echo mosttransitions | ./VectorTiming
	@printf "The figures you see above represent the size of the string, as well as the time it took to construct. The ratio of input size to construction time will experience some variance, but the goal here is to demonstrate that it stays fairly consistent with regard to input size, which is what we would expect of a linear time construction. This test uses vectors to store transitions, with linear search for transition lookup. These results are saved to mosttransitionsvectortiming.csv\n"

test4: MapTiming
	@printf "This test performs a series of suffix automaton constructions on increasingly large segments of an English Bible.\n"
	@printf "Test will commence in 5 seconds.\n"
	@sleep 5
	echo bible | ./MapTiming
	@printf "The figures you see above represent the size of the string, as well as the time it took to construct. The ratio of input size to construction time will experience some variance, but the goal here is to demonstrate that it stays fairly consistent with regard to input size, which is what we would expect of a linear time O(n) construction. This test uses map to store transitions, to avoid linear search for transition lookup. These results are saved to biblemaptiming.csv\n"

test5: MapTiming
	@print "This test performs a series of suffix automaton constructions on increasingly large strings designed to produce the maximum possible number of states in the automaton.\n"
	@printf "Test will commence in 5 seconds.\n"
	@sleep 5
	echo moststates | ./MapTiming
	@printf "The figures you see above represent the size of the string, as well as the time it took to construct. The ratio of input size to construction time will experience some variance, but the goal here is to demonstrate that it stays fairly consistent with regard to input size, which is what we would expect of a linear time construction. This test uses map to store transitions, to avoid linear search for transition lookup. These results are saved to moststatesmaptiming.csv\n"

test6: MapTiming
	@printf "This test performs a series of suffix automaton constructions on increasingly large strings designed to produce the maximum possible number of transitions between states.\n"
	@printf "Test will commence in 5 seconds."
	@sleep 5
	echo mosttransitions | ./MapTiming
	@printf "The figures you see above represent the size of the string, as well as the time it took to construct. The ratio of input size to construction time will experience some variance, but the goal here is to demonstrate that it stays fairly consistent with regard to input size, which is what we would expect of a linear time construction. This test uses map to store transitions, to avoid linear search for transition lookup. These results are saved to mosttransitionsmaptiming.csv\n"

clean:
ifeq ($(OS),Windows_NT)
	$(RM) *.exe
	$(RM) *.o
	$(RM) *.csv
else
	$(RM) $(OBJS) $(OUT)
	$(RM) *.csv
endif
	