
#include <chrono>
#include <vector>
#include <array>
#include <algorithm>
#include <string>
#include <iostream>
#define ALPHABETSIZE 256
#define MAXSIZE 100000
typedef std::pair<char, int> tr;
using namespace std;
using namespace chrono;

//A single state in our DFA, which represents an equivalence class
long long numlookups = 0;
long long lookupsizetotal = 0;


struct State {
    int first;
	int len;
	int link;
	array<int, ALPHABETSIZE> transitions{-1};
    vector<int> alltransitions;
    bool clone = false;
	vector<int> suffixreferences;
	bool terminal = false;
	int index;
	void AddTransition(char c, int i)
	{
		transitions[c] = i;
        alltransitions.push_back(c);
	}
	// Returns the index of a state or -1 if no transition exists for c
	int GetTransition(char c)
	{
		return transitions[c];
	}
	// Updates the transition through c to a new index i
	void UpdateTransition(char c, int i)
	{
		transitions[c] = i;
	}
};

struct SuffixAutomaton {
    bool suffixreferences = false;
	int size = 0;
	array<State, MAXSIZE*2> states;
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
        a.index = size;
        states[size]= a;
		size++;
        return a.index;
		
	}

	SuffixAutomaton(string s) {
		// Initial state t0 will be initialized as last
		State l;
		l.len = 0;
		l.link = -1;
		l.index = 0;
        states[size]=l;
		size++;
		int last = 0;
		for (auto& c : s)
		{
            bool done = false;
			// Create a new state for a new equivalence class
			int cur = AddState(states[last].len + 1);
            // Mark the ending position of the first occurrence of this state
			states[cur].first = states[last].len;
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
            states[clone].clone = true;
            for (auto& ind : states[q].alltransitions)
            {
                states[clone].transitions[ind] = states[q].transitions[ind];
            }
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

    // Populate each state with a vector of its children in the link tree
	void ComputeSuffixReferences()
	{
		for (int i = 1; i < size; i++)
		{
			states[states[i].link].suffixreferences.push_back(i);
		}
		suffixreferences = true;
	}
    // O(s) query to see if our source text contains a substring s
    bool contains(string s)
    {
        int i = 0;
        for (auto& c : s)
        {
            i = states[i].GetTransition(c);
            if (i == -1)
            {
                return false;
            }
        }
        return true;
    }
    // Returns the position of the first occurrence of a non-empty string s,
	// or -1 if it does not occur
	int first(string s)
	{
		int next = 0;
		for (int i = 0; i < s.size(); i++)
		{
			next = states[next].GetTransition(s[i]);
			if (next == -1) return -1;
		}
		return states[next].first - s.size() + 1;
	}
	// Return a vector of positions where a non-empty string s occurs
	vector<int> positions(string s)
	{
		vector<int> p;
		int sz = s.size();
		if (!suffixreferences) ComputeSuffixReferences();
		int next = 0;
		for (int i = 0; i < sz; i++)
		{
			next = states[next].GetTransition(s[i]);
			if (next == -1) return {};
		}
		// Traverse link tree down from first occurrence to find all others
		vector<int> stack = {next};
		while (stack.size() > 0)
		{
			next = stack.back();
			stack.pop_back();
			if (!states[next].clone) p.push_back(states[next].first - sz + 1);
			for (auto& i : states[next].suffixreferences)
			{
				stack.push_back(i);
			}
		}
		sort(p.begin(), p.end());
		return p;
	}
};

#include <fstream>
int main()
{
	string s;
	char a;
    cout << "Enter the string to construct a suffix automaton:" << endl;
    cin.get(a);
    while (a != '\n')
    {
        s.push_back(a);
        cin.get(a);
    }
	SuffixAutomaton* sa = new SuffixAutomaton(s);
	cout << "String: \"" << s << "\" is of size " << s.size() << " and has " << sa->size << " states" << endl;
    cout << "Enter a substring to see its position:" << endl;
    while (true)
    {
        string p;
        cin.get(a);
        while (a != '\n')
        {
            p.push_back(a);
            cin.get(a);
        }
		vector<int> positions = sa->positions(p);
        if (positions.size() != 0)
        {
            cout << "\"" << s << "\" contains the substring " << "\"" << p << "\" at positions";
			for (auto& i : positions)
			{
				cout << " " << i;
			}
			cout << endl;
        } else
        {
            cout << "\"" << s << "\" does not contain the substring " << "\"" << p << "\"" << endl;
        }
		cout << "Type q to quit or type any other letter to continue." << endl;
		cin.get(a);
		if (a == 'q') return 0;
    }
}