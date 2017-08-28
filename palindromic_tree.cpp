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

struct Node {
    int len;
    int link;
    vector<int> trans;
    bool all_equal;
    Node() {
        len = 0;
        link = 0;
        trans.assign(26, -1);
        all_equal = true;
    }
};

struct Fenwick {
    vector<int> t;
    int n;

    void init (int nn)
    {
        n = nn;
        t.assign (n, 0);
    }

    int sum (int r)
    {
        int result = 0;
        for (; r >= 0; r = (r & (r+1)) - 1)
            result += t[r];
        return result;
    }

    void inc (int i, int delta)
    {
        for (; i < n; i = (i | (i+1)))
            t[i] += delta;
    }

    int sum (int l, int r)
    {
        return sum (r) - sum (l-1);
    }
};

struct Eertree {
    vector<Node> nodes;
    vector<int> one_len;
    Eertree() {
        nodes.push_back(Node());
        one_len.assign(26, -1);
    }
    vector<int> feed_string(const string& s) {
        int v = 0;
        int n = s.length();
        vector<int> state(n);
        for (int i = 0; i < s.length(); ++i) {
            int c = s[i] - 'a';
            bool flag = false;
            while (v) {
                if (nodes[v].all_equal && s[i] == s[i - 1]) {
                    if (nodes[v].trans[c] == -1) {
                        nodes[v].trans[c] = nodes.size();
                        nodes.push_back(Node());
                        nodes.back().len = nodes[v].len + 1;
                        nodes.back().all_equal = true;
                        nodes.back().link = v;
                    }
                    v = nodes[v].trans[c];
                    flag = true;
                    break;
                }
                if (i > nodes[v].len && s[i] == s[i - nodes[v].len - 1]) {
                    if (nodes[v].trans[c] == -1) {
                        nodes[v].trans[c] = nodes.size();
                        nodes.push_back(Node());
                        nodes.back().len = nodes[v].len + 2;
                        nodes.back().link = -1;
                        nodes.back().all_equal = false;
                        int cur_v = nodes[v].link;
                        while (cur_v) {
                            if (nodes[cur_v].trans[c] != -1) {
                                int cand = nodes[cur_v].trans[c];
                                if (s[i] == s[i - nodes[cand].len + 1]) {
                                    nodes.back().link = nodes[cur_v].trans[c];
                                    break;
                                }
                            }
                            cur_v = nodes[cur_v].link;
                        }
                        if (nodes.back().link == -1) {
                            if (nodes[cur_v].trans[c] != -1) {
                                nodes.back().link = nodes[cur_v].trans[c];
                            } else {
                                nodes[cur_v].link = 0;
                            }
                        }
                    }
                    v = nodes[v].trans[c];
                    flag = true;
                    break;
                }
                v = nodes[v].link;
            }
            if (!flag) {
                if (one_len[c] == -1) {
                    nodes[v].trans[c] = nodes.size();
                    nodes.push_back(Node());
                    nodes.back().len = 1;
                    one_len[c] = nodes[v].trans[c];
                    nodes.back().all_equal = true;
                    nodes.back().link = 0;
                } else {
                    nodes[v].trans[c] = one_len[c];
                }
                v = nodes[v].trans[c];
            }
            state[i] = v;
            //cout << nodes[v].len << " " << nodes[nodes[v].link].len << endl;
        }
        return state;
    }

    vector<vector<int>> parent;
    vector<vector<int>> g;

    vector<int> num;
    int have_pals;

    vector<int> tin, tout;

    int timer;

    void dfs(int v) {
        tin[v] = timer++;
        for (int to : g[v]) {
            dfs(to);
        }
        tout[v] = timer;
    }

    Fenwick sum_tree;

    void clear_stat() {
        have_pals = 0;
        num.assign(nodes.size(), 0);
        sum_tree.init(nodes.size());
    }

    vector<int> max_up;

    void calc_ups() {
        parent.assign(17, vector<int>(nodes.size()));
        g.resize(nodes.size());
        for (int i = 0; i < nodes.size(); ++i) {
            parent[0][i] = nodes[i].link;
            if (i) {
                g[nodes[i].link].push_back(i);
            }
        }
        for (int i = 1; i < parent.size(); ++i) {
            for (int j = 0; j < nodes.size(); ++j) {
                parent[i][j] = parent[i - 1][parent[i - 1][j]];
            }
        }
        tin.resize(nodes.size());
        tout.resize(nodes.size());
        timer = 0;
        dfs(0);
    }

    double sum_jumps = 0.0, sum_calc_sum = 0, sum_inc = 0;

    void modify_state(int v, int len, int val) {
        //clock_t start = clock();
        for (int i = 16; i >= 0; --i) {
            if (nodes[parent[i][v]].len >= len) {
                v = parent[i][v];
            }
        }
        if (nodes[v].len > len) {
            v = parent[0][v];
        }
        assert(v);

        //sum_jumps += (clock() - start) / 1.0 / CLOCKS_PER_SEC;
        //start = clock();

        //cout << nodes[v].len << " " << val << endl;

        //cout << "update: " << v << " " << val << endl;

        int cur_val = sum_tree.sum(tin[v], tout[v] - 1);
        if (cur_val == 0 && cur_val + val > 0) {
            ++have_pals;
            //cout << "inc " << nodes[v].len << endl;
        } else if (cur_val > 0 && cur_val + val == 0) {
            --have_pals;
            //cout << "dec " << nodes[v].len << endl;
        }

        //sum_calc_sum += (clock() - start) / 1.0 / CLOCKS_PER_SEC;
        //start = clock();

        sum_tree.inc(tin[v], val);

        //sum_inc += (clock() - start) / 1.0 / CLOCKS_PER_SEC;
    }

    void print_times() {
        cout << "jumps: " << sum_jumps << endl << "calc: " << sum_calc_sum << endl << "inc: " << sum_inc << endl;
    }

    int get_res() const {
        return have_pals;
    }
};

struct Query {
    int l, r;
    int id;
    bool operator < (const Query& ot) const {
        if (r != ot.r) {
            return r < ot.r;
        }
        return l < ot.l;
    }
};

void solve(bool read) {
    read = false;
    string s;
    if (read) {
        cin >> s;
    } else {
        s = string(100000, 'a');
        for (int i = 0; i < s.length(); ++i) {
            //s[i] = (char)('a' + rand() % 26);
        }
    }

    //clock_t start = clock();
    Eertree tree;
    auto prefix_states = tree.feed_string(s);
    tree.calc_ups();
    reverse(all(s));
    auto suffix_states = tree.feed_string(s);
    reverse(all(s));
    reverse(all(suffix_states));

    //cout << (clock() - start) / 1.0 / CLOCKS_PER_SEC << endl;

    /*for (int i = 0; i < s.length(); ++i) {
        cout << tree.nodes[prefix_states[i]].len << " ";
    }
    cout << endl;

    for (int i = 0; i < s.length(); ++i) {
        cout << tree.nodes[suffix_states[i]].len << " ";
    }
    cout << endl;*/

    int Q;
    if (read) {
        cin >> Q;
    } else {
        Q = 100000;
    }
    int n = s.length();
    int block_size = max((int)(sqrt(n) * 1.5), 1);
    int blocks = (n - 1) / block_size + 1;
    vector<vector<Query>> q(blocks);

    vector<int> stupid_ans(Q);
    for (int i = 0; i < Q; ++i) {
        Query cur;
        if (read) {
            cin >> cur.l >> cur.r;
        } else {
            cur.l = 1 + rand() % n;
            cur.r = 1 + rand() % n;
            if (cur.l > cur.r) {
                swap(cur.l, cur.r);
            }
        }
        --cur.l;
        cur.id = i;
        q[cur.l / block_size].push_back(cur);

        /*set<string> her;
        for (int j = cur.l; j < cur.r; ++j) {
            for (int r = j + 1; r <= cur.r; ++r) {
                string cur_s = s.substr(j, r - j);
                string rev_s = cur_s;
                reverse(all(rev_s));
                if (rev_s == cur_s) {
                    her.insert(cur_s);
                }
            }
        }
        stupid_ans[i] = her.size();*/

    }

    vector<int> ans(Q);
    for (int block = 0; block < blocks; ++block) {
        tree.clear_stat();
        sort(all(q[block]));
        int left_border = block * block_size;
        int cur_r = left_border;
        int l = left_border;
        for (auto qu : q[block]) {
            while (cur_r < qu.r) {
                tree.modify_state(prefix_states[cur_r], cur_r + 1 - l, 1);
                ++cur_r;
            }
            while (l < qu.l) {
                tree.modify_state(suffix_states[l], qu.r - l, -1);
                ++l;
            }
            while (l > qu.l) {
                --l;
                tree.modify_state(suffix_states[l], qu.r - l, 1);
            }
            ans[qu.id] = tree.get_res();
        }
    }

    li sum_ans = 0;
    for (int i = 0; i < Q; ++i) {
        if (read) {
            cout << ans[i] << "\n";
        } else {

            /*if (ans[i] != stupid_ans[i]) {
                cout << "found " << i + 1 << endl;
                cout << "ans: " << ans[i] << " stupid: " << stupid_ans[i] << endl;
                cout << s << endl;
                for (int j = 0; j < blocks; ++j) {
                    for (auto& qu : q[j]) {
                        if (qu.id == i) {
                            cout << qu.l << " " << qu.r << endl;
                            break;
                        }
                    }
                }
                exit(0);
            }*/

            sum_ans += ans[i];
        }
    }

    if (!read) {
        cout << sum_ans << endl;
        tree.print_times();
    }

}