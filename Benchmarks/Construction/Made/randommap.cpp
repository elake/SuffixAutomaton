
#include <chrono>
#include <vector>
#include <map>
#include <string>
#include <iostream>
typedef std::pair<char, int> tr;
using namespace std;
using namespace chrono;

//A single state in our DFA, which represents an equivalence class
long long numlookups = 0;
long long lookupsizetotal = 0;


struct State {
	int len;
	int link;
	map<char, int> transitions;
	bool terminal = false;
	int index;
	void AddTransition(char c, int i)
	{
		transitions[c] = i;
	}
	// Returns the index of a state or -1 if no transition exists for c
	int GetTransition(char c)
	{
		numlookups += 1;
		lookupsizetotal += transitions.size();
		if (transitions.count(c)>0)
        {
            return(transitions[c]);
        }
        else{
           return -1; 
        }
		
	}
	// Updates the transition through c to a new index i
	void UpdateTransition(char c, int i)
	{
		transitions[c] = i;
	}
};

struct SuffixAutomaton {
	vector<State> states;
	// Returns the state at index i
	State GetState(int i)
	{
		return states[i];
	}
	// Create a new state and return its index (requires t0 already initialized)
	int AddState(int len)
	{
		State a;
		a.len = len;
		a.index = states.back().index + 1;
		states.push_back(a);
		return a.index;
	}
	SuffixAutomaton(string s) {
		// Initial state t0 will be initialized as last
		State l;
		l.len = 0;
		l.link = -1;
		l.index = 0;
        states.push_back(l);
		int last = 0;
		for (auto& c : s)
		{
            bool done = false;
			// Create a new state for a new equivalence class
			int cur = AddState(states[last].len + 1);
			// Keep following links until we find a transition through c
			int linked = last;
            int t = states[linked].GetTransition(c);
			while (t == -1)
			{
				states[linked].AddTransition(c, states[cur].index);
				if (states[linked].link != -1)
				{
					linked = states[linked].link;
					t = states[linked].GetTransition(c);
				}
				else // We have climbed the link tree to the root
				{
					// Add cur as a child of the root in the link tree and 
					// process the next character
					states[cur].link = 0;
					last = cur;
					done = true;
                    break;
				}
			}
            if (done) continue;
			// If we have reached here, we have found a state p
			// such that p transitions through c to some state q at index t
			int p = linked;
			int q = t;
			if (states[q].len == states[p].len + 1)
			{
				// Cur is a child of q in the link tree, process next character
				states[cur].link = states[q].index;
				last = cur;
				continue;
			}
			// Cur is not a child of q in the link tree, we must create a new
			// state that will be the parent of both q and cur in the link tree
			int clone = AddState(states[p].len + 1);
			states[clone].link = states[q].link;
			states[clone].transitions = states[q].transitions;
			states[cur].link = states[clone].index;
			states[q].link = states[clone].index;

			// Updates transitions through c to q to match our new state
			// TODO: Double check that p needs to be updated as well
			linked = p;
			while (t == q)
			{
				states[linked].UpdateTransition(c, clone);
				linked = states[linked].link;
				if (linked != -1)
				{
					t = states[linked].GetTransition(c);
				}
				else
				{
					break;
				}
			}
			// We are finished, advance last to the new state and continue
			last = cur;
		}

		// We now want to mark every terminal state. We start with last, as
		// it is obviously a terminal state. By climbing the suffix links, we
		// find the state that corresponds to the next largest suffix that
		// is of a different equivalence class. This will be a terminal state
		// as well. So on and so forth until we hit the root of the link tree.
		states[last].terminal = true;
		int link = states[last].link;
		while (link != -1)
		{
			int linked = link;
			states[linked].terminal = true;
			link = states[linked].link;
		}
	}
};

#include <fstream>
int main()
{
	vector<string> input;
	string current;
	vector<pair<int, long long>> results;
	ifstream file ("moststates.in");
	if (file.is_open())
	{
		while (getline (file, current))
		{
			input.push_back(current);
		}
	}
	for (auto& s : input)
	{
        auto stime = high_resolution_clock::now();
		SuffixAutomaton sa = SuffixAutomaton(s);
        auto etime = high_resolution_clock::now();
        long long duration = duration_cast<microseconds>(etime - stime).count();
        results.push_back({s.size(), duration});
		cout << "Size n:" << s.size() << " Time(microseconds): " << duration << " Ratio: "  << duration/(double)s.size() << endl;
    }
}