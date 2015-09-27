//J2 - Sometimes Naive from ptz summer 2015
//MIPT Ababahalamaha
#define _CRT_SECURE_NO_WARNINGS
#pragma comment (linker, "/STACK:128000000")
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <functional>
#include <queue>
#include <deque>
#include <cmath>
#include <ctime>
#include <stack>
#include <set>
#include <map>
#include <cassert>
#include <memory.h>
#include <sstream>

using namespace std;

#define mp make_pair
#define pb push_back
#define all(a) a.begin(), a.end()

#define forn(i, n) for (int i = 0; i < (int)(n); ++i)

typedef long long li;
typedef long long i64;
typedef double ld;
typedef vector<int> vi;
typedef pair <int, int> pi;

void solve();

void precalc();

int TESTNUM = 0;
#define FILENAME ""

int main() {
	string s = FILENAME;
#ifdef YA
	//assert(!s.empty());
	freopen("input.txt", "r", stdin);
	//freopen("output.txt", "w", stdout);
	//cerr<<FILENAME<<endl;
	//assert (s != "change me please");
	clock_t start = clock();
#else
	//freopen("input.txt", "r", stdin);
	//freopen("output.txt", "w", stdout);
	//freopen(FILENAME ".in", "r", stdin);
	//freopen(FILENAME ".out", "w", stdout); 
	cin.tie(0);
#endif
	cout.sync_with_stdio(0);
	cout.precision(10);
	cout << fixed;
	precalc();
	int t = 1;
	cin >> t;
	while (t--) {
		++TESTNUM;
		solve();
	}
#ifdef YA
	cerr << "\n\n\n" << (clock() - start) / 1.0 / CLOCKS_PER_SEC << "\n\n\n";
#endif
	return 0;
}

//#define int li

/*int pr[] = { 97, 2011 };
int mods[] = { 1000000007, 1000000009 };

const int C = 100500;
int powers[2][C];*/

int mod = 1000000007;

//int c[5010][5010];

//int catalan[200500];

//ld doubleC[100][100];

void precalc() {}


struct Node {
	Node* left, *right;
	int data;
	Node(int data) :left(0), right(0), data(data){}
};

Node* build(int l, int r, const vector <int> &t) {
	if (l == r - 1) {
		return new Node(t[l]);
	}

	int m = (l + r) / 2;

	Node* le = build(l, m, t);
	Node* re = build(m, r, t);

	Node* result = new Node((le->data + re->data) % mod);
	result->left = le;
	result->right = re;
	return result;
}

void nmodify(Node* v, int l, int r, int pos, int val, bool diff=false) {
	if (pos < l || pos >= r) {
		return;
	}
	if (l == r - 1 && pos == l) {
		if (!diff) {
			v->data = val;
		}
		else {
			v->data = (v->data + val) % mod;
		}
		return;
	}

	int m = (l + r) / 2;

	nmodify(v->left, l, m, pos, val, diff);
	nmodify(v->right, m, r, pos, val, diff);
	v->data = (v->left->data + v->right->data) % mod;
}

int getsum(int l, int r, Node* v, int L, int R) {
	if (r <= L || R <= l) {
		return 0;
	}
	if (l <= L && R <= r) {
		return v->data;
	}
	int m = (L + R) / 2;
	return (getsum(l, r, v->left, L, m) + getsum(l, r, v->right, m, R)) % mod;
}

struct tree {
	Node* root;
	int sz;
	tree(const vector <int>& t) {
		sz = t.size();
		root = build(0, t.size(), t);
	}
	void modify(int pos, int val, bool diff=false) {
		nmodify(root, 0, sz, pos, val, diff);
	}
	int getsum(int l, int r) {
		return ::getsum(l, r, root, 0, sz);
	}
};

const int maxn = 100000;
const int log_n = 17;
const int tree_root = 0;

int n;
vector < vector<int> > g;
vector <pi> hvl_path;
int sz[maxn];
int tree_pos[maxn];
int tree_end[maxn];
int depth[maxn];
//vector <vector <int> > anc;
int anc[log_n][maxn];
tree* hvl_tree[maxn];


vector <int> allv;
int cnt_sum[maxn];
int all_sum[maxn];
pair <int, int> my_ch[maxn];
int w[maxn];
int next_in_tree[maxn];
tree* my_rsq;

int m;

int dfs(int v, int p) {
	all_sum[v] = w[v];
	my_ch[v].first = allv.size();
	allv.push_back(v);

	depth[v] = p == -1 ? 0 : depth[p] + 1;
	anc[0][v] = p == -1 ? tree_root : p;
	sz[v] = 1;
	if (p != -1) {
		g[v].erase(find(g[v].begin(), g[v].end(), p));
	}
	cnt_sum[v] = 0;
	for (int to : g[v])	{
		sz[v] += dfs(to, v);
		all_sum[v] += all_sum[to];
		all_sum[v] %= mod;
		cnt_sum[v] += li(all_sum[to]) * all_sum[to] % mod;
		cnt_sum[v] %= mod;
	}

	my_ch[v].second = allv.size();
	return sz[v];
}

bool cmpSz(int u, int v) {
	return sz[u] < sz[v];
}

void build(int v) {
	auto mx = max_element(g[v].begin(), g[v].end(), cmpSz);
	if (mx == g[v].end()) {
		hvl_path.pb(mp(v, 0));

		vector <int> values(hvl_path.size());
		for (int i = 0; i < hvl_path.size(); ++i) {
			values[i] = hvl_path[i].second;
		}
		tree *t = new tree(values);

		for (size_t i = 0; i < hvl_path.size(); ++i) {
			tree_pos[hvl_path[i].first] = i;
			hvl_tree[hvl_path[i].first] = t;
			tree_end[hvl_path[i].first] = hvl_path[0].first;
		}
		hvl_path.clear();
	}
	else {
		hvl_path.pb(mp(v, (cnt_sum[v] - all_sum[*mx] * (li)all_sum[*mx]) % mod));
		next_in_tree[v] = *mx;
		build(*mx);
	}

	assert(hvl_path.empty());
	for (auto it = g[v].begin(); it != g[v].end(); ++it) {
		if (mx != it) {
			build(*it);
		}
	}
}

inline int jump(int u, int d) {
	for (int k = 0; d; d /= 2, ++k) {
		if (d & 1) {
			u = anc[k][u];
		}
	}
	return u;
}

inline int getlca(int u, int v) {
	if (depth[u] < depth[v]) {
		swap(u, v);
	}
	u = jump(u, depth[u] - depth[v]);
	if (u == v)	{
		return u;
	}
	for (int d = log_n - 1; d >= 0; --d) {
		if (anc[d][u] != anc[d][v]) {
			u = anc[d][u];
			v = anc[d][v];
		}
	}
	return anc[0][u];
}


void update_hvl(int v, int old_v, int new_v) {
	int from = -1;

	while (true) {
		if (from != -1) {
			int old_sum = my_rsq->getsum(my_ch[from].first, my_ch[from].second);
			int new_sum = (old_sum + (li)new_v - old_v) % mod;
			old_sum = (old_sum * (li)old_sum) % mod;
			new_sum = (new_sum * (li)new_sum) % mod;
			hvl_tree[v]->modify(tree_pos[v], (new_sum - old_sum) % mod, true);
		}
		if (hvl_tree[v] == hvl_tree[tree_root]) {
			break;
		}
		from = tree_end[v];
		v = anc[0][tree_end[v]];
	}
}

inline int goup(int v, int p) {
	int ans = 0, prevc = 0;
	int prevv = -1;
	while (true) {
		int l = hvl_tree[v] == hvl_tree[p] ? tree_pos[p] : 0;
		int r = tree_pos[v] + 1;

		if (next_in_tree[v] != -1) {
			int temp = my_rsq->getsum(my_ch[next_in_tree[v]].first, my_ch[next_in_tree[v]].second);
			temp = (temp * (li)temp) % mod;
			ans += temp;
			ans %= mod;
		}
		if (prevv != -1) {
			int temp = my_rsq->getsum(my_ch[prevv].first, my_ch[prevv].second);
			temp = (temp * (li)temp) % mod;
			ans = (ans - temp) % mod;
		}

		ans += hvl_tree[v]->getsum(l, r);
		ans %= mod;

		if (hvl_tree[v] == hvl_tree[p]) {
			break;
		}
		else {
			prevv = tree_end[v];
			v = anc[0][tree_end[v]];
		}
	}
	return ans;
}

void solve() {
	//cnt_sum.clear();
	allv.clear();
	//my_ch.clear();
	//w.clear();
	//hvl_tree.clear();
	//sz.clear();
	g.clear();
	//anc.clear();
	//depth.clear();
	//tree_end.clear();
	//tree_pos.clear();
	//next_in_tree.clear();

	cin >> n >> m;

	for (int i = 0; i < n; ++i) {
		next_in_tree[i] = -1;
	}
	//depth.resize(n);
	//my_ch.resize(n);
	g.resize(n);
	//tree_end.resize(n);
	//sz.resize(n);
	//cnt_sum.resize(n);
	//w.resize(n);
	//hvl_tree.resize(n);
	//tree_pos.resize(n);
	//anc.resize(log_n, vector <int>(n, tree_root));
	for (int i = 0; i < log_n; ++i) {
		for (int j = 0; j < n; ++j) {
			anc[i][j] = tree_root;
		}
	}

	for (int i = 0; i < n; ++i) {
		cin >> w[i];
	}

	for (int i = 1; i < n; ++i) {
		int a, b;
		cin >> a >> b;
		--a;
		--b;
		g[a].pb(b);
		g[b].pb(a);
	}

	dfs(0, -1);
	
	vector <int> perm_w(n);
	for (int i = 0; i < perm_w.size(); ++i) {
		perm_w[i] = w[allv[i]];
	}
	my_rsq = new tree(perm_w);

	for (int i = 1; i < log_n; ++i) {
		for (int v = 0; v < n; ++v) {
			anc[i][v] = anc[i - 1][anc[i - 1][v]];
		}
	}

	build(0);

	for (int t = 0; t < m; ++t) {
		int type;
		cin >> type;
		if (type == 1) {
			int u, new_w;
			cin >> u >> new_w;
			--u;
			
			
			update_hvl(u, w[u], new_w);
			my_rsq->modify(my_ch[u].first, new_w);
			w[u] = new_w;
		}
		else {
			int u, v;
			cin >> u >> v;
			--u;
			--v;
			int lca = getlca(u, v);
			int res;
			if (lca == u || lca == v) {
				res = goup(-lca + u + v, lca);
			}
			else {
				res = (goup(u, lca) + goup(v, lca)) % mod;
				int all_lca = hvl_tree[lca]->getsum(tree_pos[lca], tree_pos[lca] + 1);
				int hvl_next = my_rsq->getsum(my_ch[next_in_tree[lca]].first, my_ch[next_in_tree[lca]].second);
				hvl_next = (hvl_next * (li)hvl_next) % mod;
				all_lca = (all_lca + hvl_next) % mod;
				res = (res - all_lca) % mod;
			}

			int sumall = my_rsq->getsum(0, n);
			int not_intersect = (sumall - my_rsq->getsum(my_ch[lca].first, my_ch[lca].second)) % mod;
			res = (sumall * li(sumall) - res - not_intersect * (li)not_intersect) % mod;
			if (res < 0) {
				res += mod;
			}
			cout << res << "\n";
		}
	}
}