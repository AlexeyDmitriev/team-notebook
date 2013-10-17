#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <vector>

using namespace std;
typedef vector<int> vi;


void prefixFunction(const string& s, vector<int>& p) {
	if (s.length() == 0)
		return;
	p[0] = 0;
	for (size_t i = 1; i < s.length(); ++i) {
		int j = p[i - 1];
		while (j > 0 && s[i] != s[j])
			j = p[j - 1];
		if (s[i] == s[j])
			++j;
		p[i] = j;
	}
}

void zFunction(const string& s, vector<int>& z) {
	int n = s.length();
	if (n == 0)
		return;
	z[0] = 0;
	int l = 0, r = 0;
	for (int i = 1; i < n; ++i) {
		if (i <= r)
			z[i] = min(z[i - l], r - i + 1);
		while (i + z[i] < n && s[i + z[i]] == s[z[i]])
			++z[i];
		if (i + z[i] - 1 > r) {
			l = i;
			r = i + z[i] - 1;
		}
	}
}


const char first = 'a';
const int alphabet = 26;
// вылазит из массива, после того, ка совпадет все. можно добавить aut[n] = aut[pi[n - 1]]
// это сэмуирует переход по суф ссылке
vector<vi> pfautomaton(const string& s) {
	vi p(s.length());
	prefixFunction(s, p);
	vector<vi> aut(s.length(), vi(alphabet));
	for(size_t i = 0; i < s.length(); ++i)
		for(char c = 0; c < alphabet; ++c) {
			if(i > 0 && c != s[i] - first) {
				aut[i][c] = aut[p[i - 1]][c];
			}
			else
				aut[i][c] = i + (c == s[i] - first);
		}
	return aut;
}
