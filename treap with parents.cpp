enum {
	ROOT,
	LEFT_SON,
	RIGHT_SON
};

class Treap {
public:
	typedef struct _node {
		int cnt;
		int prior;
		int val;
		_node* l;
		_node* r;
		_node *p;
		int pType;
		_node(int val) :val(val), l(nullptr), r(nullptr), cnt(1), pType(ROOT) { prior = (rand() << 16) | rand(); }

		void recalc() {
			cnt = 1 + Cnt(l) + Cnt(r);
			if (l) {
				l->p = this;
				l->pType = LEFT_SON;
			}
			if (r) {
				r->p = this;
				r->pType = RIGHT_SON;
			}
			p = nullptr;
			pType = ROOT;
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
			l->r = merge(l->r, r);
			l->recalc();
			return l;
		}
		else {
			r->l = merge(l, r->l);
			r->recalc();
			return r;
		}
	}

	void split(node v, int idx, node& l, node& r) {
		l = r = nullptr;
		if (!v)
			return;
		if (Cnt(v->l) < idx) {
			l = v;
			split(l->r, idx - Cnt(v->l) - 1, l->r, r);
			l->recalc();
		}
		else {
			r = v;
			split(r->l, idx, l, r->l);
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

	void insert(int idx, int val) { //insert at the idx - position
		node l = nullptr, r = nullptr;
		split(root, idx, l, r);
		node cur_node = new _node(val);
		root = merge(merge(l, cur_node), r);
		++Size;
	}

	void erase(int idx) {
		node l = nullptr, m = nullptr, r = nullptr;
		split(root, idx, l, r);
		split(r, 1, m, r);
		root = merge(l, r);
		--Size;
	}

	int get_index(node v) {
		if (!v) {
			throw runtime_error("No such node in the treap");
		}
		int res = Cnt(v->l);
		while (v->p) {
			if (v->pType == RIGHT_SON) {
				res += Cnt(v->p->l) + 1;
			}
			v = v->p;
		}
		return res;
	}

	void push_back(int val) {
		return insert(Size, val);
	}
	void push_front(int val) {
		return insert(0, val);
	}

	node operator [] (int idx) {
		node l = nullptr, m = nullptr, r = nullptr;
		split(root, idx, l, r);
		split(r, 1, m, r);
		if (m == nullptr) {
			throw runtime_error("IndexTreapOutOfBound");
		}
		root = merge(merge(l, m), r);
		return m;
	}
};

typedef Treap::node Node;
