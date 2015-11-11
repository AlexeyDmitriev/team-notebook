
struct Point {
	int x, y;
	Point() {}
	Point(int x, int y) :x(x), y(y) {}
	Point operator - (const Point& other) const {
		return Point(x - other.x, y - other.y);
	}
	Point operator + (const Point& other) const {
		return Point(x + other.x, y + other.y);
	}
	int operator * (const Point& other) const {
		return x * other.y - y * other.x;
	}
	void scan() {
		cin >> x >> y;
	}
	int sqrDist() const {
		return x * x + y * y;
	}
	bool operator < (const Point& other) const {
		return mp(x, y) < mp(other.x, other.y);
	}
	bool operator == (const Point& other) const {
		return mp(x, y) == mp(other.x, other.y);
	}
};

bool top(const Point& cur) {
	return cur.y > 0 || cur.y == 0 && cur.x > 0;
}

bool comp(const Point& q, const Point& w) {
	if (top(q) != top(w)) {
		return top(q);
	}
	return q * w > 0;
}

bool down(const Point& q, const Point& w) {
	if (q.y != w.y) {
		return q.y < w.y;
	}
	return q.x < w.x;
}

vector<vector<Point>> points;

vector<Point> get_convex_hull(int l, int r) {
	vector<Point> all_points;
	for (int i = l; i < r; ++i) {
		for (auto item : points[i]) {
			all_points.push_back(item);
		}
	}
	sort(all(all_points));

	vector<Point> up, down;
	for (Point& pt : all_points) {
		while (up.size() > 1 && (up[up.size() - 2] - up.back()) * (pt - up.back()) <= 0) {
			up.pop_back();
		}
		up.push_back(pt);

		while (down.size() > 1 && (down[down.size() - 2] - down.back()) * (pt - down.back()) >= 0) {
			down.pop_back();
		}
		down.push_back(pt);
	}
	for (int i = (int)up.size() - 2; i > 0; --i) {
		down.push_back(up[i]);
	}
	return down;
}

vector<Point> get_minkowski_sum(const vector<Point>& fs, const vector<Point>& sc) {
	if (fs.empty()) {
		return vector<Point>();
	}
	if (sc.empty()) {
		return vector<Point>();
	}
	vector<Point> vecs;
	vector<Point> polys[2];
	polys[0] = fs;
	polys[1] = sc;
	vector<Point> sum_all(2, Point(0, 0));

	Point start(0, 0);
	for (int w = 0; w < 2; ++w) {
		Point first = polys[w][0];
		for (int i = 0; i < polys[w].size(); ++i) {
			sum_all[w] = sum_all[w] + polys[w][i];
			if (polys[w].size() > 1) {
				Point cur_vec = polys[w][(i + 1) % polys[w].size()] - polys[w][i];
				vecs.push_back(cur_vec);
			}
			if (down(polys[w][i], first)) {
				first = polys[w][i];
			}
		}
		start = start + first;
	}
	sort(all(vecs), comp);
	vector<Point> result;
	result.push_back(start);
	for (auto pt : vecs) {
		result.push_back(result.back() + pt);
	}
	assert(result.back() == result.front());
	if (result.size() > 1) {
		result.pop_back();
	}
	Point sum_result(0, 0);
	for (auto pt : result) {
		sum_result = sum_result + pt;
	}

	return result;
}

int rec(int l, int r) {
	if (l + 1 >= r) {
		return 0;
	}
	int m = (l + r) / 2;
	int res = max(rec(l, m), rec(m, r));

	vector<Point> hull1 = get_convex_hull(l, m);
	vector<Point> hull2 = get_convex_hull(m, r);
	for (auto& item : hull2) {
		item = Point(-item.x, -item.y);
	}
	//reverse(all(hull2));

	vector<Point> minkowski_sum = get_minkowski_sum(hull1, hull2);

	for (auto item : minkowski_sum) {
		res = max(res, item.sqrDist());
	}
	return res;
}

void solve() {
	int n;
	cin >> n;
	points.resize(n);
	for (int i = 0; i < n; ++i) {
		Point cur;
		cur.scan();
		int col;
		cin >> col;
		--col;
		points[col].push_back(cur);
	}

	int res = rec(0, n);

	cout << res << "\n";
}


