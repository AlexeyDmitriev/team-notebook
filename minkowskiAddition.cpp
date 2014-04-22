#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

typedef long double ld;

const ld eps = 1e-05;

struct point {
	ld x, y;
	point(){}
	point(ld x, ld y):x(x), y(y) {}
	void scan() {
		cin >> x >> y;
	}
	point operator - (const point& other) const{
		return point(x - other.x, y - other.y);
	}
	point operator + (const point& other) const{
		return point(x + other.x, y + other.y);
	}
	ld operator * (const point& other) const {
		return x * other.y - y * other.x;
	}
	bool operator < (const point& other) const {
		return x < other.x || x == other.x && y < other.y;
	}
	ld angle() {
		return atan2l(y, x);
	}
};

struct polygon {
	vector <point> a;
	polygon(){}
	polygon(const vector <point>& a):a(a) {}
	void scan() {
		int n;
		cin >> n;
		a.resize(n);
		for (int i = 0; i < n; ++i) {
			a[i].scan();
		}
	}
	ld area() {
		point last = a.back();
		ld res = 0.0;
		for (int i = 0; i < a.size(); ++i) {
			res += 0.5 * (a[i].x - last.x) * (a[i].y + last.y);
			last = a[i];
		}
		return fabsl(res);
	}
	bool in(const point& p) {
		ld sum = 0.0;
		point last = a.back();
		for (int i = 0; i < a.size(); ++i) {
			sum += 0.5 * fabsl((a[i] - last) * (last - p));
			last = a[i];
		}
		ld polygonArea = area();
		if (fabsl(sum - polygonArea) < eps)
			return true;
		return false;
	}
};

bool intersect(polygon& a, polygon& b) {
	
	polygon tmp = b;
	for (int i = 0; i < tmp.a.size(); ++i) {
		tmp.a[i] = point(0, 0) - tmp.a[i];
	}
	//reverse(tmp.a.begin(), tmp.a.end());

	polygon c;
	int pointer[2];
	polygon* polygons[2];
	polygons[0] = &a;
	polygons[1] = &tmp;
	

	for (int i = 0; i < 2; ++i) {
		pointer[i] = 0;
		for (int j = 1; j < polygons[i]->a.size(); ++j) {
			if (polygons[i]->a[j] < polygons[i]->a[pointer[i]]) {
				pointer[i] = j;
			}
		}
	}

	int j[2];
	for (int i = 0; i < 2; ++i) {
		j[i] = pointer[i];
	}
	
	bool f = true;
	while (f) {
		c.a.push_back(polygons[0]->a[j[0]] + polygons[1]->a[j[1]]);
		int newj[2];
		for (int i = 0; i < 2; ++i) 
			newj[i] = j[i];

		if ((polygons[0]->a[(j[0] + 1) % polygons[0]->a.size()] - polygons[0]->a[j[0]]).angle() - 
			(polygons[1]->a[(j[1] + 1) % polygons[1]->a.size()] - polygons[1]->a[j[1]]).angle() < -eps) {
			newj[0] = (j[0] + 1) % polygons[0]->a.size();
		}
		else {
			if ((polygons[0]->a[(j[0] + 1) % polygons[0]->a.size()] - polygons[0]->a[j[0]]).angle() - 
				(polygons[1]->a[(j[1] + 1) % polygons[1]->a.size()] - polygons[1]->a[j[1]]).angle() > eps) {
				newj[1] = (j[1] + 1) % polygons[1]->a.size();
			}
			else {
				newj[0] = (j[0] + 1) % polygons[0]->a.size();
				newj[1] = (j[1] + 1) % polygons[1]->a.size();
			}
		}
		for (int i = 0; i < 2; ++i) {
			if (newj[i] == pointer[i] && j[i] == (pointer[i] - 1 + polygons[i]->a.size()) % polygons[i]->a.size()) {
				f = false;	
			}
			j[i] = newj[i];
		}
		
	}
	if (c.in(point(0, 0))) {
		return true;
	}
	else {
		return false;
	}
}

int main() {
	//freopen("input.txt", "r", stdin);
	//freopen("output.txt", "w", stdout);
	polygon a, b;
	a.scan();
	reverse(a.a.begin(), a.a.end());
	b.scan();
	reverse(b.a.begin(), b.a.end());
	if (intersect(a, b)) {
		cout << "YES\n";
	}
	else {
		cout << "NO\n";
	}
	return 0;
}
