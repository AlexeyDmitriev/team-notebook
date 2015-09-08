#pragma comment (linker, "/STACK:1280000000")
#define _CRT_SECURE_NO_WARNINGS
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
#include <bitset>
#include <deque>
//#include <unordered_map>
#include <unordered_set>
#include <ctime>
#include <stack>
#include <queue>
#include <fstream>
#include <sstream>
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

int testNumber = 1;

bool todo = true;

vector<pair<int, int>> res;

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
	start = clock();
	int t = 1;
	cout.sync_with_stdio(0);
	cin.tie(0);
	precalc();
	cout.precision(10);
	cout << fixed;
	//cin >> t;
	int testNum = 1;
	while (t--) {
		//cout << "Case #" << testNum++ << ": ";
		solve();
		++testNumber;
		//++timer;
	}

#ifdef room111
	cerr << "\n\n" << (clock() - start) / 1.0 / CLOCKS_PER_SEC << "\n\n";
#endif

	return 0;
}

//BE CAREFUL: IS INT REALLY INT?

//#define int li

/*int pr[] = { 97, 2011 };
int mods[] = { 1000000007, 1000000009 };

const int C = 100500;
int powers[2][C];*/

//int MOD = 1000000007;

//int c[5010][5010];

//int catalan[200500];

//ld doubleC[100][100];

int binpow(int q, int w, int mod) {
	if (!w)
		return 1 % mod;
	if (w & 1)
		return q * 1LL * binpow(q, w - 1, mod) % mod;
	return binpow(q * 1LL * q % mod, w / 2, mod);
}

/*int curMod = 1000000009;

int fact[100500], revfact[100500];

int getC(int n, int k) {
int res = fact[n] * revfact[n - k] % curMod * revfact[k] % curMod;
return res;
}*/

void precalc() {
	/*fact[0] = revfact[0] = 1;
	for (int i = 1; i < 100500; ++i) {
	fact[i] = fact[i - 1] * i % curMod;
	revfact[i] = binpow(fact[i], curMod - 2, curMod);
	}*/

	/*for (int w = 0; w < 2; ++w) {
	powers[w][0] = 1;
	for (int j = 1; j < C; ++j) {
	powers[w][j] = (powers[w][j - 1] * 1LL * pr[w]) % mods[w];
	}
	}*/

	/*catalan[0] = 1;
	for (int n = 0; n < 200500 - 1; ++n) {
	catalan[n + 1] = catalan[n] * 2 * (2 * n + 1) % MOD * binpow(n + 2, MOD - 2, MOD) % MOD;
	}*/

	/*for (int i = 0; i < 5010; ++i) {
	c[i][i] = c[i][0] = 1;
	for (int j = 1; j < i; ++j) {
	c[i][j] = (c[i - 1][j - 1] + c[i - 1][j]) % MOD;
	}
	}*/

	/*for (int i = 0; i < 100; ++i) {
	doubleC[i][i] = doubleC[i][0] = 1.0;
	for (int j = 1; j < i; ++j) {
	doubleC[i][j] = doubleC[i - 1][j - 1] + doubleC[i - 1][j];
	}
	}*/

}


int gcd(int q, int w) {
	while (w) {
		q %= w;
		swap(q, w);
	}
	return q;
}

//#define int li

//int mod = 1000000007;


class Treap {
public:
	typedef struct _node {
		int cnt;
		int prior;
		int val;
		_node* l;
		_node* r;
		_node(int val) :val(val), l(nullptr), r(nullptr), cnt(1) { prior = (rand() << 16) | rand(); }

		void recalc() {
			cnt = 1 + Cnt(l) + Cnt(r);
		}

		static int Cnt(_node* v) {
			if (!v)
				return 0;
			return v->cnt;
		}
	}*node;

	static int Cnt(node v) {
		if (!v)
			return 0;
		return v->cnt;
	}

	node root;

	size_t Size;

	node merge(node l, node r) {
		if (!l)
			return r;
		if (!r)
			return l;
		if (l->prior < r->prior) {
			l->r = merge(l->r, r);
			l->recalc();
			return l;
		}
		else {
			r->l = merge(l, r->l);
			r->recalc();
			return r;
		}
	}

	void split(node v, int idx, node& l, node& r) {
		l = r = nullptr;
		if (!v)
			return;
		if (Cnt(v->l) < idx) {
			l = v;
			split(l->r, idx - Cnt(v->l) - 1, l->r, r);
			l->recalc();
		}
		else {
			r = v;
			split(r->l, idx, l, r->l);
			r->recalc();
		}
	}

public:
	Treap() {
		root = nullptr;
		Size = 0;
	}

	size_t size() const {
		return Size;
	}

	void insert(int idx, int val) { //insert at the idx - position
		node l = nullptr, r = nullptr;
		split(root, idx, l, r);
		node cur_node = new _node(val);
		root = merge(merge(l, cur_node), r);
		++Size;
	}

	void erase(int idx) {
		node l = nullptr, m = nullptr, r = nullptr;
		split(root, idx, l, r);
		split(r, 1, m, r);
		root = merge(l, r);
		--Size;
	}

	void push_back(int val) {
		return insert(Size, val);
	}
	void push_front(int val) {
		return insert(0, val);
	}

	node operator [] (int idx) {
		node l = nullptr, m = nullptr, r = nullptr;
		split(root, idx, l, r);
		split(r, 1, m, r);
		if (m == nullptr) {
			throw runtime_error("IndexTreapOutOfBound");
		}
		root = merge(merge(l, m), r);
		return m;
	}
};

typedef Treap::node Node;


void solve() {
	Treap tree;
	vector<Treap::node> nodes;
	int num_nodes = 20;
	for (int i = 0; i < num_nodes; ++i) {
		tree.push_back(i);
	}
	for (int i = 0; i < num_nodes; ++i) {
		cout << tree[i]->val << ' ';
	}
}




