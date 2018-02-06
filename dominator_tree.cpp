struct DominatorTree {
    int n;
    int root;
    vector<int> tin, revin;
    vector<int> sdom, idom;
    vector<vector<int>> g, revg;
    vector<int> parent;

    vector<int> dsu;
    vector<int> min_v;
    int cnt = 0;

    int get(int v) {
        ++cnt;
        if (dsu[v] == v) {
            return v;
        }
        int next_v = get(dsu[v]);
        if (sdom[min_v[dsu[v]]] < sdom[min_v[v]]) {
            min_v[v] = min_v[dsu[v]];
        }
        dsu[v] = next_v;
        return next_v;
    }

    void merge(int from, int to) {
        dsu[from] = to;
    }

    DominatorTree(int n, int root): n(n), root(root), dsu(n) {
        tin.resize(n, -1);
        revin.resize(n, -1);
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
        for (int to : cur_g[v]) {
            if (tin[to] == -1) {
                dfs(to, cur_g, timer);
                parent[tin[to]] = tin[v];
            }
            revg[tin[to]].push_back(tin[v]);
        }
    }

    vector<int> get_tree(vector<vector<int>> cur_g) {
        vector<char> used(n, false);
        int timer = 0;
        dfs(root, cur_g, timer);
        for (int i = 0; i < n; ++i) {
            if (tin[i] == -1) {
                continue;
            }
            revin[tin[i]] = i;
            for (int to : cur_g[i]) {
                g[tin[i]].push_back(tin[to]);
            }
        }

        vector<vector<int>> buckets(n);
        for (int i = n - 1; i >= 0; --i) {
            for (int to : revg[i]) {
                get(to);
                sdom[i] = min(sdom[i], sdom[min_v[to]]);
            }
            if (revin[i] == -1) {
                continue;
            }
            if (i) {
                buckets[sdom[i]].push_back(i);
            }
            for (int w : buckets[i]) {
                get(w);
                int v = min_v[w];
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
            if (revin[i] == -1) {
                continue;
            }
            if (idom[i] == sdom[i]) {
                continue;
            } else {
                idom[i] = idom[idom[i]];
            }
        }

        vector<int> res(n, -1);
        for (int i = 0; i < n; ++i) {
            if (revin[i] == -1) {
                continue;
            }
            res[revin[i]] = revin[idom[i]];
        }
        return res;
    }
};
