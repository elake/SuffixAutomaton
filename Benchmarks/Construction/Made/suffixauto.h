#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

typedef std::pair<char, int> tr;
using namespace std;
long long numlookups = 0;
long long lookupsizetotal = 0;
/*
A single state in our DFA, which represents an equivalence class.
Currently uses vectors to store transitions, using linear lookup.
If a string is likely to produce states with large numbers of
transitions, consider sorting the transitions and using binary
search. For now I am assuming states will have a small number
of transitions on average, and that linear search will be faster
in practice, even if it is a larger order operation.
*/
struct State {
	int len;
	int link;
	int first;
	bool clone = false;
	vector<tr> transitions;
	vector<int> suffixreferences;
	bool terminal = false;
	int index;
	void AddTransition(char c, int i);
	// Returns the index of a state or -1 if no transition exists for c
	int GetTransition(char c);
	// Updates the transition through c to a new index i
	void UpdateTransition(char c, int i);
};

struct SuffixAutomaton {
	bool suffixreferences = false;
	vector<State> states;
	// Returns the state at index i
	State GetState(int i);
	// Create a new state and return its index (requires t0 already initialized)
	int AddState(int len);
	// Populate each state with a vector of its children in the link tree
	void ComputeSuffixReferences();
    // Initialize state
    void Initialize();
	
	SuffixAutomaton(string s);
    // O(s) query to see if our source text contains a substring s
    bool contains(string s);
	// Returns the position of the first occurrence of a non-empty string s,
	// or -1 if it does not occur
	int first(string s);
	// Return a vector of positions where a non-empty string s occurs
	vector<int> positions(string s);
};