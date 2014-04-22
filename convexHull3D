#include <iostream>
#include <cmath>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <set>

using namespace std;

typedef long double ld;

const ld eps = 1e-07;

struct point {
	ld x, y, z;
	int id;
	point(){}
	point(ld x, ld y, ld z):x(x), y(y), z(z) {}
	point(ld x, ld y, ld z, int id):x(x), y(y), z(z), id(id) {}
	ld dist() const {
		return (x * x + y * y + z * z);	
	}
	void normalize() {
		ld tmp = sqrtl(dist());
		x /= tmp;
		y /= tmp;
		z /= tmp;
	}
	point operator - (const point& other) const {
		return point(x - other.x, y - other.y, z - other.z, -1);
	}
	point operator + (const point& other) const {
		return point(x + other.x, y + other.y, z + other.z, -1);
	}
	point operator * (const point& other) const {
		return point(y * other.z - other.y * z, -(x * other.z - z * other.x), x * other.y - y * other.x);
	}
	bool operator < (const point& other) const {
		return z < other.z || (z == other.z && y < other.y) || (z == other.z & y == other.y && x < other.x);
	}
	ld operator ^ (const point& other) const {
		return x * other.x + y * other.y + z * other.z;
	}
};

ld getAngle(const point& p0, const point& p1) {
	return acosl((p0 ^ p1) / sqrtl(p0.dist() * p1.dist()));
}

struct plane {
	ld a, b, c, d;
	point p[3];
	plane(){}
	plane(const point& p0, const point& p1, const point& p2) {
		p[0] = p0;
		p[1] = p1;
		p[2] = p2;
		a = (p1 - p0).y * (p2 - p0).z - (p1 - p0).z * (p2 - p0).y;
		b = -((p1 - p0).x * (p2 - p0).z - (p1 - p0).z * (p2 - p0).x);
		c = (p1 - p0).x * (p2 - p0).y - (p1 - p0).y * (p2 - p0).x;
		d = -(p0.x * a + p0.y * b + p0.z * c);		
	}
	
	void normalize(const point& tmp) {
		int sign, sign2;

		point tmp2 = point(a, b, c) + p[0];
		
		if (a * tmp.x + b * tmp.y + c * tmp.z + d < 0) {
			sign = -1;
		}
		else {
			sign = 1;
		}
		if (a * tmp2.x + b * tmp2.y + c * tmp2.z + d < 0) {
			sign2 = -1;
		}
		else {
			sign2 = 1;
		}
		if (sign == sign2) {
			a = -a;
			b = -b;
			c = -c;
			d = -d;
		}
	}

	void normalize(const vector <point>& points) {
		for (int i = points.size() - 1; i >= 0; --i) {
			if (points[i].id != p[0].id && points[i].id != p[1].id && points[i].id != p[2].id) {
				normalize(points[i]);
				break;
			}
		}

		if ((point(a, b, c) ^ ((p[1] - p[0]) * (p[2] - p[1]))) < 0) {
			swap(p[1], p[2]);
		}
	}
};

void solve();

int main() {
#ifdef YA
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);
#endif

	int t;
	t = 1;
	for (int i = 0; i < t; ++i) {
		solve();
	}
	return 0;
}

bool checkFace(const point& p0, const point& p1, const point& p2, const vector <point>& points) {
	plane face(p0, p1, p2);
	int sign = 0;
	for (int i = 0; i < points.size(); ++i) {
		if (points[i].id == p0.id || points[i].id == p1.id || points[i].id == p2.id)
			continue;

		int sign2;
		if (face.a * points[i].x + face.b * points[i].y + face.c * points[i].z + face.d < 0) {
			sign2 = -1;
		}
		else {
			sign2 = 1;
		}
		if (sign == 0) {
			sign = sign2;
		}
		if (sign != sign2) {
			return false;
		}
	}
	return true;
}

int getId(const plane& a) {
	vector <int> b;
	for (int i = 0; i < 3; ++i) {
		b.push_back(a.p[i].id);
	}
	sort(b.begin(), b.end());
	int res = 0;
	for (int i = 0; i < b.size(); ++i) {
		res = res * 1000 + b[i];
	}
	return res;
}

void solve() {
	int n;
	cin >> n;
	vector <point> input;
	input.resize(n);
	for (int i = 0; i < input.size(); ++i) {
		cin >> input[i].x >> input[i].y >> input[i].z;
		input[i].id = i;
	}

	if (n <= 3) {
		cout << "1\n";
		cout << n << " ";
		for (int i = 0; i < n; ++i) {
			cout << i << " ";
		}
		cout << "\n";
		return ;
	}

	point minP = input[0];
	for (int i = 1; i < input.size(); ++i) {
		if (input[i] < minP) {
			minP = input[i];
		}
	}
	point minP2;
	ld biggestAngle = 0.0;

	for (int i = 0; i < n; ++i) {
		if (input[i].id != minP.id) {
			ld tmpAngle = getAngle(point(0, 0, 1), input[i] - minP);
			if (tmpAngle > biggestAngle) {
				biggestAngle = tmpAngle;
				minP2 = input[i];
			}
		}
	}
	
	plane firstFace;
	
	for (int i = 0; i < n; ++i) {
		if (input[i].id != minP.id && input[i].id != minP2.id) {
			if (checkFace(input[i], minP, minP2, input)) {
				firstFace = plane(minP, minP2, input[i]);
				break;
			}
		}
	}
	
	vector <plane> convexHull;
	
	/*
	//brute force
	for (int i = 0; i < n; ++i) {
		for (int j = i + 1; j < n; ++j) {
			for (int k = j + 1; k < n; ++k) {
				if (checkFace(input[i], input[j], input[k], input)) {
					convexHull.push_back(plane(input[i], input[j], input[k]));
					convexHull.back().normalize(input);
				}
			}
		}
	}
	*/
	
	set <int> planesInConvex;
	planesInConvex.insert(getId(firstFace));
	convexHull.push_back(firstFace);
	for (int i = 0; i < convexHull.size(); ++i) {
		convexHull[i].normalize(input);
		for (int j = 0; j < 3; ++j) {
			ld minAngle = 15.0;//inf
			int minId = -1;
			for (int t = 0; t < input.size(); ++t) {
				bool good = true;
				for (int x = 0; x < 3; ++x) {
					if (convexHull[i].p[x].id == input[t].id) {
						good = false;
						break;
					}
				}
				if (!good) {
					continue;
				}
				plane tmpPlane(convexHull[i].p[j], convexHull[i].p[(j + 1) % 3], input[t]);
				tmpPlane.normalize(convexHull[i].p[(j + 2) % 3]);
				if (minAngle > getAngle(point(convexHull[i].a, convexHull[i].b, convexHull[i].c), point(tmpPlane.a, tmpPlane.b, tmpPlane.c))) {
					minAngle = getAngle(point(convexHull[i].a, convexHull[i].b, convexHull[i].c), point(tmpPlane.a, tmpPlane.b, tmpPlane.c));
					minId = t;
				}
			}
			if (minId != -1) {
				plane tmpPlane(convexHull[i].p[j], convexHull[i].p[(j + 1) % 3], input[minId]);
				tmpPlane.normalize(input);
				if (planesInConvex.find(getId(tmpPlane)) == planesInConvex.end()) {
					planesInConvex.insert(getId(tmpPlane));
					convexHull.push_back(tmpPlane);
				}
			}
		}
	}
	

	vector < vector <int> > res(convexHull.size());
	for (int i = 0; i < convexHull.size(); ++i) {
		res[i].resize(3);
		for (int j = 0; j < 3; ++j) {
			res[i][j] = convexHull[i].p[j].id;
		}
		int minId = 0;
		for (int j = 1; j < 3; ++j) {
			if (res[i][minId] > res[i][j]) {
				minId = j;
			}
		}
		vector <int> tmp(3);
		for (int j = 0; j < 3; ++j) {
			tmp[j] = res[i][(minId + j) % 3];
		}
		res[i] = tmp;
	}
	
	sort(res.begin(), res.end());
	cout << res.size() << "\n";
	for (int i = 0; i < res.size(); ++i) {
		cout << res[i].size() << " ";
		for (int j = 0; j < res[i].size(); ++j) {
			cout << res[i][j] << " ";
		}
		cout << "\n";
	}
}
