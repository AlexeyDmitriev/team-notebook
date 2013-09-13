#include <vector>
#include <iostream>
#include <memory.h>
using namespace std;
const int INF = 1000000000; // greater than max edge capacity
const int maxn = 5050;


struct edge {
	int from, to, cap, flow;
};

vector<int> g[maxn];
vector<edge> edges;

void add_edge(int from, int to, int cap) {
	edge e = {from, to, cap, 0};
	edge e2 = {to, from, 0, 0};
	g[from].push_back(edges.size());
	edges.push_back(e);
	g[to].push_back(edges.size());
	edges.push_back(e2);
}

int d[maxn];
int q[maxn];
int ptr[maxn];
int qh, qt;

bool bfs(int from, int t) {
	qh = qt = 0;
	q[qt++] = from;
	memset(d, -1, sizeof d);
	d[from] = 0;
	while(qt != qh) {
		int cur = q[qh++];
		for(int i = 0; i < (int)(g[cur].size()); ++i) {
			edge& e = edges[g[cur][i]];
			if(e.cap == e.flow)
				continue;
			if(d[e.to] == -1) {
				d[e.to] = d[cur] + 1;
				q[qt++] = e.to;
			}
		}
	}
	return d[t] != -1;
}

int dfs(int v, int t, int cap) {
	if(v == t)
		return cap;
	for(int& i = ptr[v]; i < (int)(g[v].size()); ++i) {
		int id = g[v][i];
		int to = edges[id].to;
		if(edges[id].cap == edges[id].flow)
			continue;
		if(d[v] + 1 != d[to])
			continue;
		int pushed = dfs(to, t, min(cap, edges[id].cap - edges[id].flow));
		if(pushed) {
			edges[id].flow += pushed;
			edges[id ^ 1].flow -= pushed;
			return cnt;
		}
	}
	return 0;
}

int dinic(int s, int t) {
	int fl = 0;
	while(bfs(s, t)) {
		memset(ptr, 0, sizeof ptr);
		while(int cnt = dfs(s, t, INF))
			fl += cnt;
	}
	return fl;
}

