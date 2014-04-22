#include <iostream> 
#include <cstdio> 
#include <set> 
#include <map> 
#include <vector> 
#include <queue> 
#include <stack> 
#include <cmath> 
#include <algorithm> 
#include <cstring> 
#include <bitset> 
#include <ctime> 
#include <sstream>
#include <stack> 
#include <cassert> 
#include <list> 
//#include <unordered_set> 
using namespace std;
typedef long long int64;
typedef long double ld;
typedef vector<int> vi;
typedef pair<int, int> pi;

#define mp make_pair 
#define pb push_back 
#define all(s) s.begin(), s.end() 
void solve();

#define NAME "changemeplease"
int main() {
#ifdef YA 
	cerr << NAME << endl;
	freopen("input.txt", "r", stdin);
	//freopen("output.txt", "w", stdout); 
	clock_t start = clock();
#else 
	//freopen("input.txt", "r", stdin); 
	//freopen("output.txt", "w", stdout); 
#endif 
	//ios_base::sync_with_stdio(false);
	cout << fixed;
	cout.precision(10);
	int t = 1;
	//cin >> t;	 
	while (t--)
		solve();

#ifdef YA 
	cout << "\n\n\nTime:" << ((clock() - start) / 1.0 / CLOCKS_PER_SEC);
#endif 
	return 0;
}

struct node {
	int l;
	int r;
	int push;
	int leftmost;
	int rightmost;
	int val;
	node(){}
	node(int val, int l, int r, int leftmost, int rightmost):val(val), l(l), r(r), leftmost(leftmost), rightmost(rightmost), push(-1) {}
};

int n;
vector < vector <int> > g;
vector <int> sum;
vector <int> parent;
vector <vector <int> > up;
vector <int> time_in, time_out;
vector <int> color;
vector <int> path_position;
vector < vector <int> > paths;
vector < vector <node> > trees;
vector <int> roots;
int log_n;

bool is_heavy(int from, int to) {
	if (sum[to] > sum[from] / 2) {
		return true;
	}
	return false;
}

void dfs(int v, int& timer) {
	time_in[v] = ++timer;

	if (parent[v] == -1) {
		up[v].assign(log_n, -1);
	}
	else {
		up[v].assign(log_n, -1);
		up[v][0] = parent[v];
		for (int i = 1; i < log_n; ++i) {
			if (up[v][i - 1] == -1)
				break;
			up[v][i] = up[up[v][i - 1]][i - 1];
		}
	}

	sum[v] = 1;

	for (int i = 0; i < g[v].size(); ++i) {
		int to = g[v][i];
		if (to == parent[v]) {
			continue;
		}
		parent[to] = v;
		dfs(to, timer);
		sum[v] += sum[to];
	}

	time_out[v] = ++timer;
}

int init_rsq(vector <node>& tree, int l, int r) {
	if (l == r) {
		int id = tree.size();
		tree.push_back(node(0, -1, -1, 0, 0));
		return id;
	}
	int m = (l + r) / 2;
	int left_child = init_rsq(tree, l, m);
	int right_child = init_rsq(tree, m + 1, r);
	int id = tree.size();
	tree.push_back(node(0, left_child, right_child, 0, 0));
	return id;
}

void push_modify(int i, vector <node>& tree) {
	if (tree[i].push == -1) {
		return ;
	}
	if (tree[i].l == -1) {
		tree[i].push = -1;
		return ;
	}

	tree[tree[i].l].leftmost = tree[tree[i].l].rightmost = tree[i].push;
	tree[tree[i].r].leftmost = tree[tree[i].r].rightmost = tree[i].push;
	tree[tree[i].l].push = tree[tree[i].r].push = tree[i].push;
	tree[tree[i].l].val = tree[tree[i].r].val = 0;	
	
	tree[i].push = -1;
}

void add(int l, int r, int i, vector <node>& tree, int x, int L, int R) {
	if (l > R || L > r) {
		return ;
	}
	if (l <= L && R <= r) {
		tree[i].leftmost = tree[i].rightmost = tree[i].push = x;
		tree[i].val = 0;
		return;
	}
	push_modify(i, tree);
	
	int m = (L + R) / 2;
	add(l, r, tree[i].l, tree, x, L, m);
	add(l, r, tree[i].r, tree, x, m + 1, R);
	tree[i].leftmost = tree[tree[i].l].leftmost;
	tree[i].rightmost = tree[tree[i].r].rightmost;
	tree[i].val = tree[tree[i].l].val + tree[tree[i].r].val;
	if (tree[tree[i].l].rightmost != tree[tree[i].r].leftmost)
		++tree[i].val;
}

int get_rsq(int l, int r, int i, vector <node>& tree, int L, int R, int& leftmost, int& rightmost) {
	if (l > R || L > r) {
		leftmost = rightmost = -1;
		return 0;
	}
	push_modify(i, tree);
	if (l <= L && R <= r) {
		leftmost = tree[i].leftmost;
		rightmost = tree[i].rightmost;
		return tree[i].val;
	}

	int m = (L + R) / 2;

	int left_leftmost;
	int left_rightmost;
	int right_leftmost;
	int right_rightmost;
	
	int suml = get_rsq(l, r, tree[i].l, tree, L, m, left_leftmost, left_rightmost);
	int sumr = get_rsq(l, r, tree[i].r, tree, m + 1, R, right_leftmost, right_rightmost);
	
	if (left_leftmost == -1) {
		leftmost = right_leftmost;
		rightmost = right_rightmost;
		return sumr;
	}
	if (right_leftmost == -1) {
		leftmost = left_leftmost;
		rightmost = left_rightmost;
		return suml;
	}
	
	leftmost = left_leftmost;
	rightmost = right_rightmost;
	if (left_rightmost != right_leftmost) {
		return suml + sumr + 1;
	}
	else {
		return suml + sumr;
	}
}

void build_one(int start) {
	int curc = paths.size();
	paths.push_back(vector <int>());
	trees.push_back(vector <node>());
	
	int curv = start;

	bool f = true;

	while (f) {
		paths[curc].push_back(curv);
		color[curv] = curc;
		
		f = false;
		for (int i = 0; i < g[curv].size(); ++i) {
			int to = g[curv][i];
			if (to == parent[curv]) {
				continue;
			}
			if (is_heavy(curv, to)) {
				f = true;
				curv = to;
				break;
			}
		}
	}
	
	reverse(paths[curc].begin(), paths[curc].end());
	for (int i = 0; i < paths[curc].size(); ++i) {
		path_position[paths[curc][i]] = i;
	}
	int tmp = init_rsq(trees[curc], 0, paths[curc].size() - 1); 
	roots.push_back(tmp);
}

void build_all(int v) {
	if (parent[v] == -1 || !is_heavy(parent[v], v)) {
		build_one(v);
	}
	for (int i = 0; i < g[v].size(); ++i) {
		int to = g[v][i];
		if (to == parent[v]) {
			continue;
		}
		build_all(to);
	}
}

bool upper(int v1, int v2) {
	if (time_in[v1] < time_in[v2] && time_out[v2] < time_out[v1]) {
		return true;
	}
	return false;
}

int lca(int v1, int v2) {
	if (upper(v1, v2)) {
		return v1;
	}
	if (upper(v2, v1)) {
		return v2;
	}

	for (int i = log_n - 1; i >= 0; --i) {
		if (up[v2][i] == -1)
			continue;
		if (!upper(up[v2][i], v1)) {
			v2 = up[v2][i];
		}
	}

	return parent[v2];
}

int query(int v1, int v2) {
	int ans = 0;
	int last = -1;
	int tmp1, tmp2;
	
	while (color[v1] != color[v2]) {
		ans += get_rsq(path_position[v1], paths[color[v1]].size() - 1, roots[color[v1]], trees[color[v1]], 0, paths[color[v1]].size() - 1, tmp1, tmp2);
		if (last != tmp1 && last != -1) {
			++ans;
		}
		last = tmp2;
		v1 = parent[paths[color[v1]][paths[color[v1]].size() - 1]];
	}
	
	ans += get_rsq(path_position[v1], path_position[v2], roots[color[v1]], trees[color[v1]], 0, paths[color[v1]].size() - 1, tmp1, tmp2);
	if (tmp1 != last && last != -1) {
		++ans;
	}
	return ans;
}

void update(int v1, int v2, int x) {
	int tmp1, tmp2;
	
	while (color[v1] != color[v2]) {
		add(path_position[v1], paths[color[v1]].size() - 1, roots[color[v1]], trees[color[v1]], x, 0, paths[color[v1]].size() - 1);
		v1 = parent[paths[color[v1]][paths[color[v1]].size() - 1]];
	}
	
	add(path_position[v1], path_position[v2], roots[color[v1]], trees[color[v1]], x, 0, paths[color[v1]].size() - 1);
}

void solve() {
	//в дереве нужно было находить количество различных подряд идущих на пути
	scanf("%d", &n);
	
	log_n = 0;
	while ((1 << log_n) <= n) {
		++log_n;
	}
	log_n += 2;

	path_position.resize(n);
	g.resize(n);
	up.resize(n);
	sum.resize(n);
	time_in.resize(n);
	time_out.resize(n);
	parent.resize(n);
	color.resize(n);
	//used.assign(n, 0);

	for (int i = 0; i < n - 1; ++i) {
		int x, y;
		scanf("%d %d", &x, &y);
		--x;
		--y;
		g[x].push_back(y);
		g[y].push_back(x);
	}
	parent[0] = -1;

	int timer = 0;
	dfs(0, timer);
	build_all(0);	

	int q;
	scanf("%d", &q);
	
	for (int i = 1; i <= n; ++i)
		update(i - 1, i - 1, i);

	for (int t = n + 1; t <= n + q; ++t) {
		int v1, v2, type;
		scanf("%d %d %d", &type, &v1, &v2);
		--v1;
		--v2;
		if (v1 == v2) {
			if (type == 0) {
				printf("0\n");
			}
			else {
				update(v1, v2, t);
			}
			
			continue;
		}
		int v = lca(v1, v2);
		int ans;
			
		if (v == v1 || v == v2) {
			if (v == v1) {
				swap(v1, v2);
			}
			
			if (type == 0) {
				ans = query(v1, v2);				
			}
			else {
				update(v1, v2, t);				
			}
		}
		else {
			if (type == 0) {
				int tmp1 = query(v1, v);
				int tmp2 = query(v2, v);
				ans = tmp1 + tmp2;
			}
			else {
				update(v1, v, t);
				update(v2, v, t);
			}
		}

		if (type == 0) {
			printf("%d\n", ans);
		}
	}
}
