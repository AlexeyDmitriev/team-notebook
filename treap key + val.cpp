class Treap {
public:
	typedef struct _node {
		int key;
		int cnt;
		int prior;
		int val;
		_node* l;
		_node* r;
		_node(int key, int val) :key(key), val(val), l(nullptr), r(nullptr), cnt(1) { prior = (rand() << 16) | rand(); }

		void push() {

		}

		void recalc() {
			cnt = 1 + Cnt(l) + Cnt(r);
		}

		static int Cnt(_node* v) {
			if (!v)
				return 0;
			return v->cnt;
		}
	}*node;

	static int Cnt(node v) {
		if (!v)
			return 0;
		return v->cnt;
	}

	node root;

	size_t Size;

	node merge(node l, node r) {
		if (!l)
			return r;
		if (!r)
			return l;
		if (l->prior < r->prior) {
			l->push();
			l->r = merge(l->r, r);
			l->recalc();
			return l;
		}
		else {
			r->push();
			r->l = merge(l, r->l);
			r->recalc();
			return r;
		}
	}

	void split(node v, int key, node& l, node& r) {
		l = r = nullptr;
		if (!v)
			return;
		v->push();
		if (v->key < key) {
			l = v;
			split(l->r, key, l->r, r);
			l->recalc();
		}
		else {
			r = v;
			split(r->l, key, l, r->l);
			r->recalc();
		}
	}

public:
	Treap() {
		root = nullptr;
		Size = 0;
	}

	size_t size() const {
		return Size;
	}

	node get_min() const {
		node v = root;
		if (!v) {
			throw runtime_error("Treap is empty");
		}
		while (v->l) {
			v = v->l;
		}
		return v;
	}

	node get_max() const {
		node v = root;
		if (!v) {
			throw runtime_error("Treap is empty");
		}
		while (v->r) {
			v = v->r;
		}
		return v;
	}

	void insert(int key, int val) {
		node l = nullptr, r = nullptr;
		split(root, key, l, r);
		node cur_node = new _node(key, val);
		root = merge(merge(l, cur_node), r);
		++Size;
	}

	node operator [] (int key) {
		node l = nullptr, m = nullptr, r = nullptr;
		split(root, key, l, r);
		split(r, key + 1, m, r);
		if (m == nullptr) {
			throw runtime_error("IndexTreapOutOfBound");
		}
		root = merge(merge(l, m), r);
		return m;
	}
};

typedef Treap::node Node;
