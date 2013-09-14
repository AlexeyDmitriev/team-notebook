#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <vector>
using namespace std;

void prefixFunction(const string& s, vector<int>& p) {
	if (s.length() == 0)
		return;
	p[0] = 0;
	for (int i = 1; i < s.length(); ++i) {
		int j = p[i - 1];
		while (j > 0 && s[i] != s[j])
			j = p[j - 1];
		if (s[i] == s[j])
			++j;
		p[i] = j;
	}
}

void zFunction(const string& s, vector<int>& z) {
	if (s.length() == 0)
		return;
	z[0] = 0;
	int l = 0, r = 0;
	for (int i = 1; i < s.length(); ++i) {
		if (i <= r)
			z[i] = min(z[i - l], r - i + 1);
		while (i + z[i] < s.length() && s[i + z[i]] == s[z[i]])
			++z[i];
		if (i + z[i] - 1 > r) {
			l = i;
			r = i + z[i] - 1;
		}
	}
}






