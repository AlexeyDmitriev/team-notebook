#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <algorithm>
#include <string>
#include <string.h>
#include <vector>
#include <set>
#include <map>
#include <complex>

using namespace std;

#define double long double
#define pb push_back
#define ppb pop_back

const double eps = 1e-12;
const double inf = 1e10;
const double pi = acosl((double)-1);

struct point{
	double x, y;
	int num;
	point(){}
	point(double xx, double yy, int nnum){
		x = xx;
		y = yy;
		num = nnum;
	}
};

struct line{
	double a, b, c, ang;
	int num;
};

const point vertex[4] = {point(-inf, -inf, 0), point(inf, -inf, 0), point(inf, inf, 0), point(-inf, inf, 0)};

inline bool qq(double a, double b){
	return fabs(a - b) < eps;
}

inline bool lq(double a, double b){
	return b - a > -eps;
}

inline bool ls(double a, double b){
	return b - a >= eps;
}

inline double sqr(double a){
	return a * a;
}

inline double len(const point &a){
	return sqr(a.x) + sqr(a.y);
}

inline double vec(const point &a, const point &b){
	return a.x * b.y - b.x * a.y;
}

inline double dist(line &l, point &p){
	return l.a * p.x + l.b * p.y + l.c;
}

point operator + (const point &a, const point &b){
	return point(a.x + b.x, a.y + b.y, a.num);
}

point operator - (const point &a, const point &b){
	return point(a.x - b.x, a.y - b.y, a.num);
}

point operator / (const point &a, int size){
	return point(a.x / size, a.y / size, a.num);
}

bool operator < (const point &a, const point &b){
	double v = vec(a, b);
	if(qq(v, 0)){
		return len(a) < len(b);
	}
	return v > 0;
}

bool operator == (const line &a, const line &b){
	return qq(a.ang, b.ang);
}

bool operator < (const line &a, const line &b){
	if(!qq(a.ang, b.ang)){
		return a.ang < b.ang;
	}
	return a.c < b.c;
}

inline bool better(point &a, point &b){
	if(!qq(a.x, b.x)){
		return a.x < b.x;
	}
	return a.y < b.y;
}

inline point intersect(line &l1, line &l2){
	double d = l1.a * l2.b - l2.a * l1.b;
	return point((l1.b * l2.c - l2.b * l1.c) / d, (l2.a * l1.c - l1.a * l2.c) / d, 0);
}

inline line getline(const point &a, const point &b){
	line result;
	result.a = a.y - b.y;
	result.b = b.x - a.x;
	result.c = vec(a, b);
	return result;
}

inline bool bad(point &a, point &b, point &c){
	point v1 = b - a, v2 = c - a;
	double v = vec(v1, v2);
	if(qq(v, 0)){
		return len(v1) < len(v2);
	}
	return v < 0;
}

vector <point> getconvex(vector <point> points){
	for(int i = 1; i < (int)points.size(); i++){
		if(better(points[i], points[0])){
			swap(points[0], points[i]);
		}
	}
	for(int i = 1; i < (int)points.size(); i++){
		points[i] = points[i] - points[0];
	}
	sort(points.begin() + 1, points.end());
	for(int i = 1; i < (int)points.size(); i++){
		points[i] = points[i] + points[0];
	}
	vector <point> convex;
	for(int i = 0; i < (int)points.size(); i++){
		while(convex.size() >= 2 && bad(convex[convex.size() - 2], convex[convex.size() - 1], points[i])){
			convex.ppb();
		}
		convex.pb(points[i]);
	}
	return convex;
}

vector <line> getintersection(vector <line> lines){
	for(int i = 0; i < (int)lines.size(); i++){
		lines[i].ang = atan2l(lines[i].b, lines[i].a);
	}
	sort(lines.begin(), lines.end());
	lines.resize(unique(lines.begin(), lines.end()) - lines.begin());
	int size = lines.size();
	for(int i = 0; i < size; i++){
		lines[i].num = i;
	}
	for(int i = 0; i < size; i++){
		line curline = lines[i];
		curline.ang += 2 * pi;
		lines.pb(curline);
	}
	vector <line> intersection;
	for(int i = 0; i < (int)lines.size(); i++){
		while(intersection.size() >= 2){
			if(lq(pi, lines[i].ang - intersection[intersection.size() - 2].ang)){
				break;
			}
			point curpoint = intersect(intersection[intersection.size() - 2], lines[i]);
			if(!lq(0, dist(intersection[intersection.size() - 1], curpoint))){
				break;
			}
			intersection.ppb();
		}
		intersection.pb(lines[i]);
	}
	vector <int> last(size, -1);
	vector <line> result;
	for(int i = 0; i < (int)intersection.size(); i++){
		if(last[intersection[i].num] == -1){
			last[intersection[i].num] = i;
			continue;
		}
		for(int j = last[intersection[i].num]; j < i; j++){
			result.pb(intersection[j]);
		}
		break;
	}
	return result;
}

inline point readpoint(int num){
	point result;
	scanf("%Lf %Lf", &result.x, &result.y);
	result.num = num;
	return result;
}

inline void fail(){
	printf("Impossible\n");
	exit(0);
}

int main(){
	//freopen("tests.in", "r", stdin);
	//freopen("tests.out", "w", stdout);
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);
	int n;
	scanf("%d", &n);
	vector <point> points;
	for(int i = 0; i < n; i++){
		points.pb(readpoint(i));
	}
	vector <line> lines;
	for(int i = 0; i < n - 1; i++){
		lines.pb(getline(points[i], points[i + 1]));
	}
	vector <point> convex = getconvex(points);
	for(int i = 0; i < (int)convex.size(); i++){
		point a = convex[i], b = convex[(i + 1) % convex.size()];
		if(a.num > b.num){
			swap(a, b);
		}
		lines.pb(getline(a, b));
	}
	for(int i = 0; i < 4; i++){
		lines.pb(getline(vertex[i], vertex[(i + 1) % 4]));
	}
	vector <line> intersection = getintersection(lines);
	if(intersection.size() == 0){
		fail();
	}
	point ans(0, 0, 0);
	for(int i = 0; i < (int)intersection.size(); i++){
		point curpoint = intersect(intersection[i], intersection[(i + 1) % intersection.size()]);
		ans = ans + curpoint;
	}
	ans = ans / intersection.size();
	bool good = true;
	for(int i = 0; i < n - 1; i++){
		line curline = getline(points[i], points[i + 1]);
		if(!ls(0, dist(curline, ans))){
			good = false;
			break;
		}
	}
	if(!good){
		fail();
	}
	printf("Possible\n%.18Lf %.18Lf\n", ans.x, ans.y);
	return 0;
}
