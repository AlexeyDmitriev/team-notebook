#pragma comment (linker, "/STACK:128000000")
//#include "testlib.h"
#include <cstdio>
#include <cassert>
#include <algorithm>
#include <iostream>
#include <memory.h>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <cmath>
//#include <unordered_map>
//#include <unordered_set>
#include <ctime>
#include <stack>
#include <queue>
using namespace std;
//#define FILENAME ""
#define mp make_pair
#define all(a) a.begin(), a.end()
typedef long long li;
typedef long double ld;
void solve();
void precalc();
clock_t start;
//int timer = 1;


int main() {
#ifdef room111
    freopen("in.txt", "r", stdin);
	//freopen("out.txt", "w", stdout);
#else
	//freopen("input.txt", "r", stdin);
	//freopen("output.txt", "w", stdout);
    //freopen(FILENAME".in", "r", stdin);
    //freopen(FILENAME ".out", "w", stdout);
#endif
    int t = 1;
	cout.sync_with_stdio(0);
	precalc();
	cout.precision(10);
	cout << fixed;
	//cin >> t;
	start = clock();
	int testNum = 1;
    while (t--) {
		//cout << "Case #" << testNum++ << ": ";
        solve();
		//++timer;
	}

#ifdef room111
	cerr << "\n\n" << (clock() - start) / 1.0 / CLOCKS_PER_SEC << "\n\n";
#endif

    return 0;
}

//BE CAREFUL: IS INT REALLY INT?

//#define int li

void precalc() {
	
}

int gcd (int q, int w) {
	while(w) {
		q %= w;
		swap(q, w);
	}
	return q;
}

int binpow (int q, int w, int mod) {
	if (!w)
		return 1 % mod;
	if (w & 1) {
		return (q * binpow(q, w - 1, mod)) % mod;
	}
	return binpow((q * q) % mod, w / 2, mod);
}

const int UNDEFINED_VALUE = -1;

class SuffixAutomaton {
public:
	struct State {
		map<char, int> transitions;
		int link;
		int maxLen;
		int firstPos, lastPos;
		int cnt;
		State():link(UNDEFINED_VALUE), firstPos(UNDEFINED_VALUE), lastPos(UNDEFINED_VALUE), maxLen(0), cnt(0) {}
	};
	vector<State> states;
	int lastState;
	SuffixAutomaton(const string& s) {
		states.push_back(State());
		lastState = 0;
		for (int i = 0; i < s.length(); ++i)
			append(s[i]);
		vector<pair<int, int>> p(states.size());
		for (int i = 0; i < p.size(); ++i) {
			p[i].second = i;
			p[i].first = states[i].maxLen;
		}
		sort(all(p));
		reverse(all(p));
		for (int i = 0; i < p.size(); ++i) {
			int curState = p[i].second;
			if (states[curState].lastPos == UNDEFINED_VALUE)
				states[curState].lastPos = states[curState].firstPos;
			if (states[curState].link != UNDEFINED_VALUE) {
				states[states[curState].link].lastPos = max(states[states[curState].link].lastPos, states[curState].lastPos);
				states[states[curState].link].cnt += states[curState].cnt;
			}
		}

	}

	int countDoubleOccurrences() {
		li res = 0;
		for (int curState = 0; curState < states.size(); ++curState) {
			int maxLen = states[curState].maxLen;
			int minLen = 1;
			if (states[curState].link != UNDEFINED_VALUE)
				minLen = states[states[curState].link].maxLen + 1;
			maxLen = min(states[curState].lastPos - states[curState].firstPos, maxLen);
			res += max(maxLen - minLen + 1, 0);
		}
		return res;
	}

	vector<int> getSimilarityClasses() {
		vector<int> res;
		for (int curState = 0; curState < states.size(); ++curState) {
			int maxLen = states[curState].maxLen;
			int minLen = -1;
			if (states[curState].link != UNDEFINED_VALUE)
				minLen = states[states[curState].link].maxLen;
			res.push_back(maxLen - minLen);
		}
		sort(all(res));
		return res;
	}

	int findInterestingSubstring(int k) {
		int res = 0;
		for (int curState = 0; curState < states.size(); ++curState) {
			int maxLen = states[curState].maxLen;
			if (states[curState].cnt >= k)
				res = max(res, maxLen);
		}
		return res;
	}

	void dfs(int v, string curString = "") {
		if (curString.size() != states[v].maxLen)
			return;
		cout << v << ' ' << states[v].maxLen << ' ' << curString << ' ' << states[v].firstPos << ' '<< states[v].lastPos << endl;
		for (auto to : states[v].transitions)
			dfs(to.second, curString + to.first);
	}

private:
	void append(char c) {
		int curState = states.size();
		states.push_back(State());
		states[curState].maxLen = states[lastState].maxLen + 1;
		states[curState].firstPos = states[lastState].maxLen;
		states[curState].cnt = 1;
		int prevState = lastState;
		for (; prevState != UNDEFINED_VALUE; prevState = states[prevState].link) {
			if (states[prevState].transitions.count(c))
				break;
			states[prevState].transitions[c] = curState;
		}

		if (prevState == UNDEFINED_VALUE) {
			states[curState].link = 0;
		}
		else {
			int nextState = states[prevState].transitions[c];
			if (states[nextState].maxLen == states[prevState].maxLen + 1) {
				states[curState].link = nextState;
			}
			else {
				int cloneState = states.size();
				states.push_back(State());
				states[cloneState].maxLen = states[prevState].maxLen + 1;
				states[cloneState].link = states[nextState].link;
				states[cloneState].firstPos = states[nextState].firstPos;
				states[curState].link = states[nextState].link = cloneState;

				states[cloneState].transitions = states[nextState].transitions;
				for (; prevState != UNDEFINED_VALUE && states[prevState].transitions[c] == nextState; prevState = states[prevState].link)
					states[prevState].transitions[c] = cloneState;
			}
		}
		lastState = curState;
	}

};

void solve() {
	int n, k;
	string s;
	cin >> n >> k;
	cin >> s;
	SuffixAutomaton automaton(s);

	//automaton.dfs(0);

	/*vector<int> similarityClasses = automaton.getSimilarityClasses();

	for (int item : similarityClasses)
		cout << item << ' ';*/

	cout << automaton.findInterestingSubstring(k);

}


