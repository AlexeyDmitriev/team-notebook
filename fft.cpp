#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <time.h>
#include <vector>
#include <list>
#include <cassert>
#include <set>
#include <map>
#include <string>
#include <queue>
#include <cstdio>
#include <cmath>
#include <memory>
using namespace std;
 
typedef long long li;
 
#define all(a) a.begin(), a.end()
#define mp make_pair
 
template <typename T>
class myComplex {
private:
	T x, y;
public:
	myComplex() {}
	myComplex (T x, T y):x(x), y(y) {}
	myComplex(T x):x(x), y(0) {}
	T getReal() {
		return x;
	}
	T getImag() {
		return y;
	}
	T getSqrDist() {
		return x * x + y * y;
	}
	double getDist() {
		return sqrt(1.0 * getSqrDist());
	}
	myComplex operator + (const myComplex& other) const { return myComplex(x + other.x, y + other.y); }
	myComplex operator - () { return myComplex(-x, -y); }
	myComplex operator - (const myComplex& other) const { return myComplex(x - other.x, y - other.y); }
	myComplex operator * (T number) { return myComplex(x * number, y * number); }
	myComplex operator / (T number) { return myComplex(x / number, y / number); }
	myComplex operator * (const myComplex& other) const { return myComplex(x * other.x - y * other.y, x * other.y + y * other.x); }
	void print() const {
		cout << x;
		if (y > 0)
			cout << "+";
		if (y != 0)
			cout << y << "i";
		cout << "\n";
	}
 
};
 
typedef myComplex<double> complex;
 
void fft (vector<complex>& input, bool invert) {
	int curSize = input.size();
	if (curSize == 1)
		return;
	vector<complex> rec[2];
	for (int w = 0; w < 2; ++w)
		rec[w].resize(curSize / 2);
	for (int i = 0; i < input.size(); i += 2) {
		rec[0][i / 2] = input[i];
		rec[1][i / 2] = input[i + 1];
	}
 
	for (int w = 0; w < 2; ++w)
		fft(rec[w], invert);
	
	double alpha = 2 * acos(-1.0) / curSize * (invert ? -1 : 1);
	complex wn = complex(cos(alpha), sin(alpha));
	complex w = 1;
 
	for (int i = 0; i < curSize / 2; ++i) {
		input[i] = rec[0][i] + w * rec[1][i];
		input[i + curSize / 2] = rec[0][i] - w * rec[1][i];
		if (invert) {
			input[i] = input[i] / 2;
			input[i + curSize / 2] = input[i + curSize / 2] / 2;
		}
		w = w * wn;
	}
 
}
 
vector <double> multiply(const vector<double>& a, const vector<double>& b) {
	int n = 1;
	while (n <= a.size() || n <= b.size())
		n <<= 1;
	n <<= 1;
	vector<complex> input[2];
	for (int w = 0; w < 2; ++w)
		input[w].assign(n, complex(0,0));
	for (int i = 0; i < a.size(); ++i)
		input[0][i] = a[i];
	for (int i = 0; i < b.size(); ++i)
		input[1][i] = b[i];
	for (int w = 0; w < 2; ++w) {
		fft(input[w], false);
	}
	vector <complex> res(n);
	for (int i = 0; i < n; ++i)
		res[i] = input[0][i] * input[1][i];
	fft(res, true);
	vector<double> answer;
	for (int i = 0; i < n; ++i)
		answer.push_back(res[i].getReal());
	return answer;
}
 
vector <double> naive (vector<double> a, vector<double> b) {
	vector<double> res(a.size() + b.size() - 1);
	for (int i = 0; i < a.size(); ++i)
		for (int j = 0; j < b.size(); ++j)
			res[i + j] += a[i] * b[j];
	return res;
}
 
int main() {
#ifdef PROGA
	freopen("in.txt", "r", stdin);
#endif
	int t;
	//t = 1;
	cin >> t;
	int MAGIC = 5;
	int base = 100000;
	while (t--) {
		vector<double> a[2];
		for (int w = 0; w < 2; ++w) {
			string s;
			cin >> s;
			reverse(all(s));
			for (int i = 0; ; i += MAGIC) {
				int cur = 0;
				for (int w = min(i + MAGIC, (int)s.length()) - 1; w >= i; --w)
					cur = cur * 10 + (int)(s[w] - '0');
				a[w].push_back(cur);
				if (i + MAGIC >= s.length())
					break;
			}
		}
		vector<double> resFFT = multiply(a[0], a[1]);
		vector <long long> res;
		for (int i = 0; i < resFFT.size(); ++i)
			res.push_back(resFFT[i] + 0.5);
		for (int i = 0; i + 1 < res.size(); ++i) {
			res[i + 1] += res[i] / base;
			res[i] %= base;
		}
		bool flag = false;
		int first = -1;
		for (int i = res.size() - 1; i >= 0; --i) {
			if (res[i] != 0) {
				flag = true;
				if (first == -1)
					first = i;
			}
			if (flag) {
				if (i != first) {
					for (int w = base / 10; w > 1; w /= 10)
						if (res[i] < w)
							printf("0");
				}
				printf("%d", res[i]);
			}
		}
		if (!flag)
			printf("0");
		printf("\n");
	}
} 