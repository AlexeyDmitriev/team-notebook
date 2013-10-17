#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <iostream>

using namespace std;

class suffixTree {
private:
	struct node {
		int l, r, parent, link;
		map<char, int> next;

		node (int l = 0, int r = 0, int parent = -1): l(l), r(r), parent(parent), link(-1) {}

		int length() { return r - l; }

		int& get (char c) {
			if (next.find(c) == next.end())
				next[c] = -1;
			return next[c];
		}

	};

	vector<node> tree;

	struct state {
		int v, pos;
		state(int v = 0, int pos = 0):v(v), pos(pos) {}
	};

	string s;
	size_t n;
	state ptr;

	state go (state st, int l, int r) {
		while (l < r) {
			if (st.pos == tree[st.v].length()) {
				st = state(tree[st.v].get(s[l]), 0);
				if (st.v == -1)
					return st;
			}
			else {
				if (s[tree[st.v].l + st.pos] != s[l])
					return state(-1, -1);
				if (r - l < tree[st.v].length() - st.pos)
					return state(st.v, st.pos + r - l);
				l += tree[st.v].length() - st.pos;
				st.pos = tree[st.v].length();
			}
		}
		return st;
	}

	int split (state st) {
		if (st.pos == tree[st.v].length())
			return st.v;
		if (st.pos == 0)
			return tree[st.v].parent;

		node edgeEnd = tree[st.v];
		int splitId = tree.size();
		tree.push_back(node(edgeEnd.l, edgeEnd.l + st.pos, edgeEnd.parent));
		tree[edgeEnd.parent].get(s[edgeEnd.l]) = splitId;
		tree[splitId].get(s[edgeEnd.l + st.pos]) = st.v;

		tree[st.v].parent = splitId;
		tree[st.v].l += st.pos;

		return splitId;
	}

	int getLink (int v) {
		if (tree[v].link != -1)
			return tree[v].link;
		if (tree[v].parent == -1)
			return 0;
		int to = getLink(tree[v].parent);
		state nex = go(state(to, tree[to].length()), tree[v].l + (tree[v].parent == 0), tree[v].r);
		return tree[v].link = split(nex);
	}

	void extend (int pos) {
		while (true) {
			state nexPtr = go(ptr, pos, pos + 1);
			if (nexPtr.v != -1) {
				ptr = nexPtr;
				return;
			}

			int middleNode = split(ptr);
			int leafNum = tree.size();
			tree.push_back(node(pos, n, middleNode));
			tree[middleNode].get(s[pos]) = leafNum;

			ptr.v = getLink(middleNode);
			ptr.pos = tree[ptr.v].length();
			if (!middleNode)
				break;
		};
	}

	void build () {
		tree.push_back(node());
		for (size_t i = 0; i < n; ++i)
			extend(i);
	}

public:

	suffixTree(string S) {
		s = S;
		s += '$';
		n = s.length();
		build();
	}

	void dfs (int v = 0, int p = -1) {
		if (v == -1)
			return;
		if (tree[v].l + 1 == s.length())
			return;
		for (int i = tree[v].l; i < tree[v].r; ++i)
			cout << s[i];
		cout << "\n";
		cout << "v = " << v << " parent = " << p << "\n";
		for (map<char, int>::iterator it = tree[v].next.begin(); it != tree[v].next.end(); ++it) {
			dfs(it->second, v);
		}
	}

	bool existsOccurence (const string& query) {
		int v = 0;
		int done = 0;
		while (done < query.length()) {
			if (!tree[v].next.count(query[done]))
				return false;
			int to = tree[v].next[query[done]];
			if (to == -1)
				return false;
			for (int i = tree[to].l; i < tree[to].r; ++i) {
				if (i - tree[to].l + done >= query.length())
					return true;
				if (query[i + done - tree[to].l] != s[i])
					return false;
			}
			done += tree[to].length();
			v = to;
		}
		return true;
	}

};