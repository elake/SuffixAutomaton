#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
typedef std::pair<char, int> tr;
using namespace std;

// A single state in our DFA, which represents an equivalence class.
struct State {
	int len;
	int link;
	int first;
	bool clone = false;
	vector<tr> transitions;
	vector<int> suffixreferences;
	bool terminal = false;
	int index;
	void AddTransition(char c, int i)
	{
		transitions.push_back(tr(c, i));
	}
	// Returns the index of a state or -1 if no transition exists for c
	int GetTransition(char c)
	{
		for (auto& t : transitions)
		{
			if (t.first == c)
			{
				return t.second;
			}
		}
		return -1;
	}
	// Updates the transition through c to a new index i
	void UpdateTransition(char c, int i)
	{
		for (auto& t : transitions)
		{
			if (t.first == c)
			{
				t.second = i;
				return;
			}
		}
	}
};

struct SuffixAutomaton {
	bool suffixreferences = false;
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
	// Populate each state with a vector of its children in the link tree
	void ComputeSuffixReferences()
	{
		for (int i = 1; i < states.size(); i++)
		{
			states[states[i].link].suffixreferences.push_back(i);
		}
		suffixreferences = true;
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
			states[clone].transitions = states[q].transitions;
			states[clone].first = states[q].first;
			states[clone].clone = true;
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
    string current;
	vector<pair<string, string>> source;
    vector<vector<pair<string, int>>> search;
    int o;
	vector<vector<string>> results;
	ifstream file ("positions.in");
	if (file.is_open())
	{
		while (getline (file, current))
		{ // Get the pair<title, body> for each source text
            pair<string, string> src;
			src.first = current;
            getline(file, current);
            src.second = current;
			source.push_back(src);
            file >> o; // Get the number of searches for this text
			getline(file, current);
            vector<pair<string, int>> schs;
            for (int i = 0; i < o; i++)
            { // Get the pair<string, numoccurrences> for each search
				int oo;
                pair<string, int> sch;
                getline(file, current);
                sch.first = current;
                file >> oo;
				getline(file, current);
                sch.second = oo;
                schs.push_back(sch);

            }
			search.push_back(schs);
		}
        file.close();
	}
	for (int i = 0; i < source.size(); i++)
	{
		cout << "Constructing an automaton of size " << source[i].second.size() << " for " << source[i].first << "..." << endl;
		SuffixAutomaton sa = SuffixAutomaton(source[i].second);
		cout << "Computing suffix references for " << sa.states.size() << " states..." << endl;
        for (int t = 0; t < search[i].size(); t++)
        {
            vector<int> positions = sa.positions(search[i][t].first);
            string passed = "passed";
            // Check that we got the correct number of positions
            if (positions.size() != search[i][t].second) passed = "failed";
            for (int j = 0; j < positions.size(); j++)// For each reported position:
            { 
                for (int k = 0; k < search[i][t].first.size(); k++)
                { // Check that the position contains the search term
                    if (search[i][t].first[k] != source[i].second[positions[j]+k]) passed = "failed";
                }
            }
            results.push_back({source[i].first, search[i][t].first, to_string(positions.size()), to_string(search[i][t].second), passed});
            if (passed == "passed")
            {
                cout << "PASSED: Searching for \"" << search[i][t].first << "\" in " << source[i].first << " found " << positions.size() << " of " << search[i][t].second << " positions and all matched the substring." << endl;
            }
			else
			{
				cout << "FAILURE. Well that's embarassing. I swear it worked on my machine :)" << endl;
			}
        }
        
		
    }
	ofstream sr("positionsresults.csv");
	if (sr.is_open())
	{
		sr << "Source Title:" << ",Search String:" << ",Found:" << ",Expected:" << ",Result:" << endl;
		for (auto& x : results)
		{
			sr << x[0] << "," << x[1] << "," << x[2] << "," << x[3] << "," << x[4] << endl;
		}
        sr.close();
	}
}