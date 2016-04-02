const int shift = 1 << 19;

struct Node {
	
};

Node tree[2 * shift];

void push(int v) {
	for (int h = 0; h < 2; ++h) {
		
	}
}

Node merge(const Node& l, const Node& r) {
	Node res;
	
	return res;
}

Node rmq(int v, int tl, int tr, int l, int r) {
	if (tr <= l || r <= tl) {
		return ;
	}
	if (l <= tl && tr <= r) {
		return tree[v];
	}
	push(v);
	int tm = (tl + tr) / 2;
	Node ans = merge(rmq(2 * v, tl, tm, l, r), rmq(2 * v + 1, tm, tr, l, r));
	tree[v] = merge(tree[2 * v], tree[2 * v + 1]);
	return ans;
}


Node get_min(int l, int r) {
	return rmq(1, 0, shift, l, r);
}

void modify(int v, int tl, int tr, int l, int r, int val) {
	if (tr <= l || r <= tl) {
		return;
	}
	if (l <= tl && tr <= r) {
		
		return;
	}
	push(v);
	int tm = (tl + tr) / 2;
	modify(2 * v, tl, tm, l, r, val);
	modify(2 * v + 1, tm, tr, l, r, val);
	tree[v] = merge(tree[2 * v], tree[2 * v + 1]);
}

void update(int l, int r, int val) {
	return modify(1, 0, shift, l, r, val);
}

void build(int v, int tl, int tr, const vector<int>& vals) {
	if (tl >= vals.size()) {
		return;
	}
	if (tl + 1 == tr) {
		tree[v].mn = vals[tl];
		tree[v].add = 0;
		return;
	}
	int tm = (tl + tr) / 2;
	build(2 * v, tl, tm, vals);
	build(2 * v + 1, tm, tr, vals);
	tree[v] = merge(tree[2 * v], tree[2 * v + 1]);
}

void build(const vector<int>& vals) {
	return build(1, 0, shift, vals);
}
