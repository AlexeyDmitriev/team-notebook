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

void precalc() {}

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

struct DominatorTree {
    int n;
    vector<int> tin, revin;
    vector<int> sdom, idom;
    vector<vector<int>> g, revg;
    vector<int> parent;

    vector<int> dsu;
    vector<int> min_v;

    int get(int v) {
        //cout << v << " " << dsu[v] << endl;
        if (dsu[v] == v) {
            return v;
        }
        int next_v = get(dsu[v]);
        if (sdom[min_v[dsu[v]]] < sdom[min_v[v]]) {
            min_v[v] = min_v[dsu[v]];
        }
        dsu[v] = next_v;
        return min_v[v];
    }

    void merge(int from, int to) {
        //cout << "merge " << from << " " << to << endl;
        dsu[from] = to;
    }

    DominatorTree(int n): n(n), dsu(n) {
        tin.resize(n, -1);
        revin.resize(n);
        sdom.resize(n);
        idom.resize(n);
        g.resize(n);
        revg.resize(n);
        dsu.resize(n);
        parent.assign(n, -1);
        min_v.assign(n, -1);
        for (int i = 0; i < n; ++i) {
            dsu[i] = i;
            min_v[i] = i;
            sdom[i] = i;
            idom[i] = i;
        }
    }

    void dfs(int v, vector<vector<int>>& cur_g, int& timer) {
        tin[v] = timer++;
        //cout << v + 1 << " ";
        for (int to : cur_g[v]) {
            if (tin[to] == -1) {
                dfs(to, cur_g, timer);
                parent[tin[to]] = tin[v];
            }
            revg[tin[to]].push_back(tin[v]);
        }
    }

    vector<int> get_tree(vector<vector<int>> cur_g) {
        g = cur_g;
        vector<char> used(n, false);
        int timer = 0;
        dfs(0, cur_g, timer);
        for (int i = 0; i < n; ++i) {
            revin[tin[i]] = i;
            for (int to : cur_g[i]) {
                g[tin[i]].push_back(tin[to]);
            }
        }

        vector<vector<int>> buckets(n);
        for (int i = n - 1; i >= 0; --i) {
            for (int to : revg[i]) {
                sdom[i] = min(sdom[i], sdom[get(to)]);
            }
            if (i) {
                buckets[sdom[i]].push_back(i);
            }
            for (int w : buckets[i]) {
                int v = get(w);
                if (sdom[v] == sdom[w]) {
                    idom[w] = sdom[w];
                } else {
                    idom[w] = v;
                }
            }
            for (int to : g[i]) {
                if (parent[to] == i) {
                    merge(to, i);
                }
            }
        }

        for (int i = 0; i < n; ++i) {
            //cout << sdom[i] + 1 << " \n"[i + 1 == n];
        }

        for (int i = 0; i < n; ++i) {
            if (idom[i] == sdom[i]) {
                continue;
            } else {
                idom[i] = idom[idom[i]];
            }
        }

        vector<int> res(n, -1);
        for (int i = 0; i < n; ++i) {
            res[revin[i]] = revin[idom[i]];
        }
        return res;
    }
};

vector<vector<int>> g;
vector<vector<int>> parent;
vector<int> tin, tout;
vector<int> cost;

int TIMER = 0;

void dfs(int v, int p) {
    tin[v] = TIMER++;
    parent[0][v] = p;
    for (int i = 1; i < parent.size(); ++i) {
        parent[i][v] = parent[i - 1][parent[i - 1][v]];
    }
    for (int to : g[v]) {
        cost[to] = min(cost[to], cost[v]);
        dfs(to, v);
    }
    tout[v] = TIMER++;
}

bool upper(int a, int b) {
    return tin[a] <= tin[b] && tout[a] >= tout[b];
}

int lca(int a, int b) {
    if (upper(a, b)) {
        return a;
    }
    for (int i = (int)parent.size() - 1; i >= 0; --i) {
        if (!upper(parent[i][a], b)) {
            a = parent[i][a];
        }
    }
    return parent[0][a];
}

void solve(bool read) {
    int n, m;
    cin >> n >> m;
    g.resize(n);
    for (int i = 0; i < m; ++i) {
        int a, b;
        cin >> a >> b;
        --a; --b;
        g[a].push_back(b);
    }
    parent.assign(17, vector<int>(n));
    parent[0] = DominatorTree(n).get_tree(g);
    g.clear();
    g.resize(n);
    tin.resize(n);
    tout.resize(n);
    for (int i = 1; i < n; ++i) {
        g[parent[0][i]].push_back(i);
    }
    cost.resize(n);
    for (int i = 0; i < n; ++i) {
        cin >> cost[i];
    }

    dfs(0, 0);

    int Q;
    cin >> Q;
    while (Q--) {
        int k;
        cin >> k;
        int v;
        cin >> v;
        --v;
        for (int i = 0; i < k - 1; ++i) {
            int u;
            cin >> u;
            --u;
            v = lca(u, v);
        }
        cout << cost[v] << "\n";
    }


}
