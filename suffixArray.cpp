#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <queue>
#include <cassert>

using namespace std;

void Build(const string& init, vector <int>& suffArray, vector <int>& lcp) {
	string s = init;
	s.push_back(char(0));
	/*����� ����� ��������� ������� ������ ��������� ������� � ���������� ������� ����� ���������� �� 1-�� �������*/
	vector <int> head;
	/*color[i] - ����� ��������, ������������� � i-�� �������*/
	vector <int> color;
	/*�� ��, ��� � color, ����� �������������� �� ����� ����������*/
	vector <int> colorSub;
	vector <int> suffArraySub;
	/*256 - ������������ ������ ��������*/
	head.assign(max((int)s.size(), 256), 0); 
	suffArray.resize(s.size());
	color.resize(s.size());
	colorSub.resize(s.size());
	suffArraySub.resize(s.size());
	lcp.resize(s.size());

	/*��������� ���������� ��������� �� ������� �������*/
	for (int i = 0; i < s.size(); ++i) {
		++head[s[i]];
	}
	for (int i = 1; i < 256; ++i) {
		head[i] += head[i - 1];
	}
	for (int i = 255; i > 0; --i) {
		head[i] = head[i - 1];
	}
	head[0] = 0;
	for (int i = 0; i < s.size(); ++i) {
		suffArray[head[s[i]]] = i;
		++head[s[i]];
	}
	/*� suffArray �������� ������������� �� ������� �������*/

	/*��������� �������� �� ������, �������� head � �����������, ������� � ������*/
	int numberOfClasses = 1;
	head[0] = 0;
	for (int i = 1; i < s.size(); ++i) {
		if (s[suffArray[i - 1]] != s[suffArray[i]]) {
			++numberOfClasses;
			head[numberOfClasses - 1] = i;
		}
		color[suffArray[i]] = numberOfClasses - 1;
	}

	/*����������*/
	for (int k = 1; k < s.size(); k *= 2) {
		/*���������� �� ���������� ����� 2*k*/

		for (int i = 0; i < s.size(); ++i) {
			int firstPartBeginning = suffArray[i] - k;
			if (firstPartBeginning < 0) {
				firstPartBeginning += s.size();
			}
			suffArraySub[head[color[firstPartBeginning]]] = firstPartBeginning;
			++head[color[firstPartBeginning]];
		}
		suffArray = suffArraySub;

		/*������ color � head*/
		int secondPartBeginning;
		pair <int, int> prevSuffClasses, curSuffClasses;
		curSuffClasses = make_pair(-1, 0);
		numberOfClasses = 0;

		for (int i = 0; i < s.size(); ++i) {
			prevSuffClasses = curSuffClasses;
			
			secondPartBeginning = suffArray[i] + k;
			if (secondPartBeginning >= s.size()) {
				secondPartBeginning -= s.size();
			}
			curSuffClasses = make_pair(color[suffArray[i]], color[secondPartBeginning]);
		
			if (curSuffClasses != prevSuffClasses) {
				++numberOfClasses;
				head[numberOfClasses - 1] = i;
			}
			colorSub[suffArray[i]] = numberOfClasses - 1;
		}

		color = colorSub;

		/*���� ����� ������� �������� ���������� ��������� -> � suffArray ���������� ����������*/
		if (numberOfClasses == s.size())
			break;
	}

	/*�������� ����� ��� ���������� lcp*/
	/*pos[i] - ������� ��������, ������������� � ������� i � suffArray*/
	vector <int> pos; 
	int curLcp = 0;
	pos.resize(s.size());
	for (int i = 0; i < s.size(); ++i) {
		pos[suffArray[i]] = i;
	}

	for (int i = 0; i < s.size(); ++i) {
		if (pos[i] == s.size() - 1) {
			lcp[pos[i]] = 0;
			curLcp = 0;
			continue;
		}

		while (s[(i + curLcp) % s.size()] == s[(suffArray[pos[i] + 1] + curLcp) % s.size()]) {
			++curLcp;
		}
		lcp[pos[i]] = curLcp;

		--curLcp;
		if (curLcp < 0)
			curLcp = 0;
	}
}

/*���������� lcp ��� ������ a �� ������� aShift � ������ b �� ������� bShift*/
int GetBruteLcp(const string& a, int aShift, const string& b, int bShift) {
	for (int i = 0;;++i) {
		if (i + aShift == a.size()) {
			return i;
		}
		if (i + bShift == b.size()) {
			return i;
		}
		if (a[i + aShift] != b[i + bShift]) {
			return i;
		}
	}
}

/*���������� sparse table �� �������� ��� ������� �*/
/*� precalc: precalc[i] - ������������ ������� ������ <= i*/
void BuildSparseTable(const vector <int>& a, vector < vector <int> >& sparseTable, vector <int>& precalc) {
	precalc.resize(a.size() + 1);
	precalc[1] = precalc[0] = 0;
	for (int i = 2; i <= a.size(); ++i) {
		precalc[i] = precalc[i - 1];
		if ((1 << (precalc[i] + 1)) <= i)
			++precalc[i];
	}

	int logSize = 0;
	while ((1 << logSize) < a.size()) {
		++logSize;	
	}
	sparseTable.assign(a.size(), vector <int> (logSize + 1));

	for (int i = 0; i < a.size(); ++i) {
		sparseTable[i][0] = a[i];
	}

	for (int k = 1; k <= logSize; ++k) {
		for (int i = 0; i + (1 << k) <= a.size(); ++i) {
			sparseTable[i][k] = min(sparseTable[i][k - 1], sparseTable[i + (1 << (k - 1))][k - 1]);
		}
	}
}

int GetMin(int l, int r, const vector < vector <int> >& sparseTable, const vector <int>& precalc) {
	assert(l < r);
	--r;
	return min(sparseTable[l][precalc[r - l + 1]], sparseTable[r - (1 << precalc[r - l + 1]) + 1][precalc[r - l + 1]]);
}

/* ���������� ���� ��������� ������ t � ������ s*/
/* suffArray - ���� ������ �� s*/
/* sparseTable �� lcp ��� suffArray*/
/* precalc ��� sparseTable*/
/* � positions - ������ ���� ��������� t � s*/
void find(const string& s, const string& t, const vector <int>& suffArray, 
		  const vector < vector <int> >& sparseTable, const vector <int>& precalc, vector <int>& positions) {
	
	/*����� ������ 2 ���������: ������ ��� ���������� ������������ ��������, ������������� � t
								������ ��� ���������� ������������� ��������, ������������� � t*/
	int leftBorder, rightBorder;
	/*leftBorder - ������� ������������ ��������, ������������� � t, � �����������*/
	/*rightBorder - ������� ������������� ��������, ������������� � t, � �����������*/
	int l = 0;
	int r = suffArray.size() - 1;
	/*mlrLeft - lcp ������ t � ������������ ��������*/
	/*mlrRight - lcp ������ t � ������������� ��������*/
	int mlrLeft = GetBruteLcp(s, suffArray.front(), t, 0);
	int mlrRight = GetBruteLcp(s, suffArray.back(), t, 0);

	/*������ ��������*/
	while (true) {
		if (mlrLeft == t.size()) {
			/*������ ����� �����*/
			leftBorder = l;
			break;
		}
		if (l == r) {
			/*������ �� ���� ������� �� ���������� � t*/
			return ;
		}
		if (l == r - 1) {
			if (mlrRight == t.size()) {
				/*������ ����� �����*/
				leftBorder = r;
				break;
			}
			else {
				/*������ �� ���� ������� �� ���������� � t*/
				return ;
			}
		}

		/*������ middle != l � middle != r*/
		int middle = (l + r) / 2;
		/*lcpLeftMiddle - lcp ��������, ������������ �� ������� l � suffArray, 
							� ��������, ������������ �� ������� middle � suffArray*/
		/*lcpRightMiddle - lcp ��������, ������������ �� ������� r � suffArray, 
							� ��������, ������������ �� ������� middle � suffArray*/
		int lcpLeftMiddle = GetMin(l, middle, sparseTable, precalc);
		int lcpRightMiddle = GetMin(middle, r, sparseTable, precalc);
		/*mlrMiddle - lcp ������ t � ��������, ������������ �� ������� middle � suffArray*/
		int mlrMiddle = max(min(lcpLeftMiddle, mlrLeft), min(lcpRightMiddle, mlrRight));
		/*��, ��������, ��� ����� ���������*/
		mlrMiddle += GetBruteLcp(s, suffArray[middle] + mlrMiddle, t, mlrMiddle);
		if (mlrMiddle == t.size()) {
			/*����� ����� ����� ���������� ������ ����� l � middle*/
			r = middle;
			mlrRight = mlrMiddle;
			continue;
		}
		
		/*less = true, ���� �������, ������������ �� ������� middle � suffArray, ������ ������ t*/
		bool less = false;
		if (suffArray[middle] + mlrMiddle == s.size() || s[suffArray[middle] + mlrMiddle] < t[mlrMiddle]) {
			less = true;	
		}
		if (less) {
			/*����� ����� ������ ����� middle � r*/
			l = middle;
			mlrLeft = mlrMiddle;
		}
		else {
			/*����� ����� ������ ����� l � middle*/
			r = middle;
			mlrRight = mlrMiddle;
		}
	}	
	
	l = 0;
	r = suffArray.size() - 1;
	mlrLeft = GetBruteLcp(s, suffArray.front(), t, 0);
	mlrRight = GetBruteLcp(s, suffArray.back(), t, 0);
	/*������ ��������*/
	while (true) {
		if (mlrRight == t.size()) {
			rightBorder = r;
			break;
		}
		if (l == r) {
			return ;
		}
		if (l == r - 1) {
			if (mlrLeft == t.size()) {
				rightBorder = l;
				break;
			}
			else {
				return ;
			}
		}

		int middle = (l + r) / 2;
		int mlrMiddle;
		int lcpLeftMiddle = GetMin(l, middle, sparseTable, precalc);
		int lcpRightMiddle = GetMin(middle, r, sparseTable, precalc);
		mlrMiddle = max(min(lcpLeftMiddle, mlrLeft), min(lcpRightMiddle, mlrRight));
		mlrMiddle += GetBruteLcp(s, suffArray[middle] + mlrMiddle, t, mlrMiddle);
		if (mlrMiddle == t.size()) {
			l = middle;
			mlrLeft = mlrMiddle;
			continue;
		}

		bool less = false;
		if (suffArray[middle] + mlrMiddle == s.size() || s[suffArray[middle] + mlrMiddle] < t[mlrMiddle]) {
			less = true;	
		}
		if (less) {
			l = middle;
			mlrLeft = mlrMiddle;
		}
		else {
			r = middle;
			mlrRight = mlrMiddle;
		}
	}

	for (int i = leftBorder; i <= rightBorder; ++i) {
		positions.push_back(suffArray[i]);
	}
}

int main() {
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);
	string s;
	getline(cin, s);
	vector <int> suffArray, lcp;
	Build(s, suffArray, lcp);
	vector < vector <int> > sparseTable;
	vector <int> precalc;
	BuildSparseTable(lcp, sparseTable, precalc);

	/*for (int i = 0; i < suffArray.size(); ++i)
		cout << suffArray[i] << " " << lcp[i] << endl;

	return 0;*/

	int k;
	k = 1;
	//cin >> k;
	for (int i = 0; i < k; ++i) {
		string t;
		getline(cin, t);
		vector <int> result;
		find(s, t, suffArray, sparseTable, precalc, result);
		sort(result.begin(), result.end());
		for (int j = 0; j < result.size(); ++j)
			cout << result[j] << " ";
		cout << endl;
	}
	return 0;
}