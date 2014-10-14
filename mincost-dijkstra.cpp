
const int NEED_FLOW = 1000000000;
typedef int int_e;

struct edge {
	int from, to;
	int cap;
	int_e cost;
	char c;
	int flow;
};

vector<edge> edges;
vector<vector<int>> g;

void add_edge(int from, int to, int_e cost, int cap, char c = 0) {
	//cout << from << ' ' << to << ' ' << cost <<  ' ' << cap << ' ' << c << "\n";
	edge e = {from, to, cap, cost, c, 0};
	g[from].push_back(edges.size());
	edges.push_back(e);
	edge e2 = {to, from, 0, -cost, c, 0};
	g[to].push_back(edges.size());
	edges.push_back(e2);
}

pi mincost(int n, int s, int t) {
	int_e cost = 0;
	int flow = 0;
	vector<int> potential;
	{
		vector<int> p(n, -1);
		vector<int_e> d(n);
		d[s] = 0;
		p[s] = s;
		bool changed = true;
		while(changed) {
			changed = false;
			for(size_t i = 0; i < edges.size(); ++i) {
				edge& e = edges[i];
				if(e.cap == e.flow || p[e.from] == -1)
					continue;
				if(p[e.to] == -1 || d[e.to] > d[e.from] + e.cost) {
					d[e.to] = d[e.from] + e.cost;
					p[e.to] = i;
					changed = true;
				}
			}
		}
		potential = std::move(d);
	}
	while(flow < NEED_FLOW) {
		
		//if(d[t] >= 0) { // only for mincost, not mincostmaxflow
		//	break;
		//}
		
		vector<int_e> d(n);
		vector<int_e> p(n, -1);
		
		
		priority_queue<pi, vector<pi>, greater<pi>> q;
		
		q.push(mp(0, s));
		
		while(!q.empty()) {
			int v = q.top().second;
			int oldD = q.top().first;
			q.pop();
			if(oldD != d[v])
				continue;
			if(v == t)
				break;
			for(int id: g[v]) {
				edge& e = edges[id];
				if(e.cap > e.flow) {
					int newd = d[v] + e.cost + potential[e.from] - potential[e.to];
					if(p[e.to] == -1 || d[e.to] > newd) {
						d[e.to] = newd;
						p[e.to] = id;
						q.push(mp(d[e.to], e.to));
					}
				}
			}
		}
		
		if(p[t] == -1)
			break;

		int cur = t;
		int maxAdd = NEED_FLOW - flow;
		while(cur != s) {
			edge& e = edges[p[cur]];
			cur = e.from;
			maxAdd = min(maxAdd, e.cap - e.flow);
		}

		flow += maxAdd;
		cost += (potential[t] + d[t]) * maxAdd;
		cur = t;
		while(cur != s) {
			int id = p[cur];
			edges[id].flow += maxAdd;
			edges[id ^ 1].flow -= maxAdd;
			cur = edges[id].from;
		}
	}

	// cost and flow are final here
	return mp(cost, flow);
}
