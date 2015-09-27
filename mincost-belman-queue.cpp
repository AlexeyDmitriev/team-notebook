const int NEED_FLOW = 1000000000;
typedef double int_e;

struct edge {
	int from, to;
	int cap;
	int_e cost;
	int flow;
};

vector<vector<int>> g;
vector<edge> edges;

void add_edge(int from, int to, int_e cost, int cap) {
	edge e = { from, to, cap, cost, 0 };
	g[from].push_back(edges.size());
	edges.push_back(e);
	edge e2 = { to, from, 0, -cost, 0 };
	g[to].push_back(edges.size());
	edges.push_back(e2);
}

pair<int, int_e> mincost(int n, int s, int t) {
	int_e cost = 0;
	int flow = 0;
	while (flow < NEED_FLOW) {
		queue<int> q;
		q.push(s);
		vector<int> in_q(n, 0);
		in_q[s] = 1;
		vector<int> p(n, -1);
		vector<int_e> d(n);
		d[s] = 0;
		p[s] = s;
		while (!q.empty()) {
			int v = q.front();
			q.pop();
			in_q[v] = false;
			for (size_t i: g[v]) {
				edge& e = edges[i];
				if (e.cap == e.flow || p[e.from] == -1)
					continue;
				if (p[e.to] == -1 || d[e.to] > d[e.from] + e.cost) {
					d[e.to] = d[e.from] + e.cost;
					p[e.to] = i;
					if (!in_q[e.to]) {
						in_q[e.to] = 1;
						q.push(e.to);
					}
				}
			}
		}
		if (p[t] == -1)
			break;

		//if(d[t] >= 0) { // only for mincost, not mincostmaxflow
		//	break;
		//}

		int cur = t;
		int maxAdd = NEED_FLOW - flow;
		while (cur != s) {
			edge& e = edges[p[cur]];
			cur = e.from;
			maxAdd = min(maxAdd, e.cap - e.flow);
		}

		flow += maxAdd;
		cost += d[t] * maxAdd;
		cur = t;
		while (cur != s) {
			int id = p[cur];
			edges[id].flow += maxAdd;
			edges[id ^ 1].flow -= maxAdd;
			cur = edges[id].from;
		}
	}

	// cost and flow are final here
	return make_pair(flow, cost);
}
