#pragma comment(linker, "/STACK:512000000")
#define _CRT_SECURE_NO_WARNINGS
//#include "testlib.h"
#include <bits/stdc++.h>
using namespace std;

#define all(a) a.begin(), a.end()
typedef long long li;
typedef long double ld;
void solve(bool);
void precalc();
clock_t start;
int main() {
#ifdef AIM
  freopen("/home/alexandero/ClionProjects/ACM/input.txt", "r", stdin);
  //freopen("/home/alexandero/ClionProjects/ACM/output.txt", "w", stdout);
  //freopen("out.txt", "w", stdout);
#else
  //freopen("input.txt", "r", stdin);
  //freopen("output.txt", "w", stdout);
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
    //cerr << testNum << endl;
    solve(true);
    //cerr << testNum - 1 << endl;
  }
  cout.flush();
#ifdef AIM1
  while (true) {
      solve(false);
  }
#endif

#ifdef AIM
  cerr << "\n\n time: " << (clock() - start) / 1.0 / CLOCKS_PER_SEC << "\n\n";
#endif

  return 0;
}

//BE CAREFUL: IS INT REALLY INT?

template<typename T>
T binpow(T q, T w, T mod) {
  if (!w)
    return 1 % mod;
  if (w & 1)
    return q * 1LL * binpow(q, w - 1, mod) % mod;
  return binpow(q * 1LL * q % mod, w / 2, mod);
}

template<typename T>
T gcd(T q, T w) {
  while (w) {
    q %= w;
    swap(q, w);
  }
  return q;
}
template<typename T>
T lcm(T q, T w) {
  return q / gcd(q, w) * w;
}

template <typename T>
void make_unique(vector<T>& a) {
  sort(all(a));
  a.erase(unique(all(a)), a.end());
}

void precalc() {

}

template<typename T>
void relax_min(T& cur, T val) {
  cur = min(cur, val);
}

template<typename T>
void relax_max(T& cur, T val) {
  cur = max(cur, val);
}

//#define int li
//const int mod = 1000000007;

struct Edge {
    int from, to, cap, flow;
};

const int INF = (int)2e9;

struct Dinic {
    int n;
    vector<Edge> edges;
    vector<vector<int>> g;

    Dinic(int n) : n(n) {
        g.resize(n);
    }

    void add_edge(int from, int to, int cap) {
        Edge e = {from, to, cap, 0};
        g[from].push_back(edges.size());
        edges.push_back(e);
        e = {to, from, 0, 0};
        g[to].push_back(edges.size());
        edges.push_back(e);
    }

    vector<int> d;

    bool bfs(int s, int t) {
        d.assign(n, INF);
        d[s] = 0;
        queue<int> q;
        q.push(s);
        while (!q.empty()) {
            int v = q.front();
            q.pop();
            for (auto id : g[v]) {
                auto e = edges[id];
                if (e.cap > e.flow && d[e.to] == INF) {
                    d[e.to] = d[v] + 1;
                    q.push(e.to);
                }
            }
        }
        return d[t] != INF;
    }

    vector<int> pointer;

    int dfs(int v, int t, int flow_add) {
        if (!flow_add) {
            return 0;
        }
        if (v == t) {
            return flow_add;
        }
        int added_flow = 0;
        for (int& i = pointer[v]; i < g[v].size(); ++i) {
            int id = g[v][i];
            int to = edges[id].to;
            if (d[to] != d[v] + 1) {
                continue;
            }
            int pushed = dfs(to, t, min(flow_add, edges[id].cap - edges[id].flow));
            if (pushed) {
                edges[id].flow += pushed;
                edges[id ^ 1].flow -= pushed;
                return pushed;
            }
        }
        return 0;
    }

    int max_flow(int s, int t) {
        int flow = 0;
        while (bfs(s, t)) {
            pointer.assign(n, 0);
            while (int pushed = dfs(s, t, INF)) {
                flow += pushed;
            }
        }
        return flow;
    }
};

struct RawEdge {
    int from, to, has_flow, id;
};

void solve(bool read) {
    int n, m, s, t;
    cin >> n >> m >> s >> t;
    --s; --t;
    vector<RawEdge> raw_edges(m);
    vector<vector<RawEdge>> raw_g(n), rev_raw_g(n);
    auto dinic = Dinic(n);
    for (int i = 0; i < m; ++i) {
        cin >> raw_edges[i].from >> raw_edges[i].to >> raw_edges[i].has_flow;
        --raw_edges[i].from;
        --raw_edges[i].to;
        raw_edges[i].id = i;
        if (raw_edges[i].has_flow) {
            raw_g[raw_edges[i].from].push_back(raw_edges[i]);
            rev_raw_g[raw_edges[i].to].push_back(raw_edges[i]);
        }
    }

    for (int i = 0; i < m; ++i) {
        if (!raw_edges[i].has_flow) {
            dinic.add_edge(raw_edges[i].from, raw_edges[i].to, INF);
        } else {
            dinic.add_edge(raw_edges[i].to, raw_edges[i].from, INF);
            dinic.add_edge(raw_edges[i].from, raw_edges[i].to, 1);
        }
    }

    int res_flow = dinic.max_flow(s, t);

    vector<int> parent_edge_s(n, -1), parent_edge_t(n, -1);

    {
        queue<int> q;
        q.push(s);
        while (!q.empty()) {
            int v = q.front();
            q.pop();
            for (auto e : raw_g[v]) {
                if (parent_edge_s[e.to] == -1) {
                    parent_edge_s[e.to] = e.id;
                    q.push(e.to);
                }
            }
        }
    }

    {
        queue<int> q;
        q.push(t);
        while (!q.empty()) {
            int v = q.front();
            q.pop();
            for (auto e : rev_raw_g[v]) {
                if (parent_edge_t[e.from] == -1) {
                    parent_edge_t[e.from] = e.id;
                    q.push(e.from);
                }
            }
        }
    }

    vector<int> edge_flow(m, 0);
    for (int i = 0; i < m; ++i) {
        if (!raw_edges[i].has_flow || edge_flow[i] > 0) {
            continue;
        }
        int add = 2 + rand() % 5;
        int from = raw_edges[i].from, to = raw_edges[i].to;
        if ((from == s || parent_edge_s[from] != -1) && (to == t || parent_edge_t[to] != -1)) {
            edge_flow[i] += add;
            int cur = parent_edge_s[from];
            while (cur != -1) {
                edge_flow[cur] += add;
                cur = parent_edge_s[raw_edges[cur].from];
            }
            cur = parent_edge_t[to];
            while (cur != -1) {
                edge_flow[cur] += add;
                cur = parent_edge_t[raw_edges[cur].to];
            }
        }
    }

    for (int i = 0; i < m; ++i) {
        if (!raw_edges[i].has_flow || edge_flow[i] > 0) {
            continue;
        }
        int from = raw_edges[i].from, to = raw_edges[i].to;
        queue<int> q;
        q.push(to);
        vector<int> parent_edge(n, -1);
        while (!q.empty()) {
            int v = q.front();
            q.pop();
            for (auto e : raw_g[v]) {
                if (parent_edge[e.to] == -1 && e.to != to) {
                    parent_edge[e.to] = e.id;
                    q.push(e.to);
                }
            }
        }
        assert(parent_edge[from] != -1);

        int add = 2 + rand() % 5;
        edge_flow[i] += add;
        int cur = parent_edge[from];
        while (cur != -1) {
            edge_flow[cur] += add;
            cur = parent_edge[raw_edges[cur].from];
        }
    }

    vector<int> edge_cap(m, 0);
    {
        queue<int> q;
        vector<bool> left_part(n, false);
        left_part[s] = true;
        q.push(s);
        while (!q.empty()) {
            int v = q.front();
            q.pop();
            for (auto e : dinic.g[v]) {
                if (left_part[dinic.edges[e].to]) {
                    continue;
                }
                if (dinic.edges[e].flow < dinic.edges[e].cap) {
                    left_part[dinic.edges[e].to] = true;
                    q.push(dinic.edges[e].to);
                }
            }
        }

        for (int i = 0; i < m; ++i) {
            if (left_part[raw_edges[i].from] && !left_part[raw_edges[i].to]) {
                edge_cap[i] = edge_flow[i];
            } else {
                edge_cap[i] = edge_flow[i] + 1 + rand() % 5;
            }
        }
    }

    cout << res_flow << endl;
    for (int i = 0; i < m; ++i) {
        cout << edge_flow[i] << " " << edge_cap[i] << endl;
    }

}
