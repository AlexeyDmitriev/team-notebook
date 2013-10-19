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
	/*здесь будут храниться индексы первых элементов классов в суффиксном массиве после сортировки по 1-му символу*/
	vector <int> head;
	/*color[i] - класс суффикса, начинающегося с i-го символа*/
	vector <int> color;
	/*то же, что и color, будет использоваться во время сортировки*/
	vector <int> colorSub;
	vector <int> suffArraySub;
	/*256 - максимальный символ алфавита*/
	head.assign(max((int)s.size(), 256), 0); 
	suffArray.resize(s.size());
	color.resize(s.size());
	colorSub.resize(s.size());
	suffArraySub.resize(s.size());
	lcp.resize(s.size());

	/*выполняем сортировку суффиксов по первому символу*/
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
	/*в suffArray суффиксы отсортированы по первому символу*/

	/*разбиваем суффиксы на классы, приводим head к определению, данному в начале*/
	int numberOfClasses = 1;
	head[0] = 0;
	for (int i = 1; i < s.size(); ++i) {
		if (s[suffArray[i - 1]] != s[suffArray[i]]) {
			++numberOfClasses;
			head[numberOfClasses - 1] = i;
		}
		color[suffArray[i]] = numberOfClasses - 1;
	}

	/*сортировка*/
	for (int k = 1; k < s.size(); k *= 2) {
		/*сортировка по подстрокам длины 2*k*/

		for (int i = 0; i < s.size(); ++i) {
			int firstPartBeginning = suffArray[i] - k;
			if (firstPartBeginning < 0) {
				firstPartBeginning += s.size();
			}
			suffArraySub[head[color[firstPartBeginning]]] = firstPartBeginning;
			++head[color[firstPartBeginning]];
		}
		suffArray = suffArraySub;

		/*строим color и head*/
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

		/*если число классов достигло количества суффиксов -> в suffArray правильный суффмассив*/
		if (numberOfClasses == s.size())
			break;
	}

	/*алгоритм Касаи для построения lcp*/
	/*pos[i] - позиция суффикса, начинающегося с символа i в suffArray*/
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

/*нахождение lcp для строки a со сдвигом aShift и строки b со сдвигом bShift*/
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

/*построение sparse table по минимуму для вектора а*/
/*в precalc: precalc[i] - максимальная степень двойки <= i*/
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

/* нахождение всех вхождений строки t в строку s*/
/* suffArray - суфф массив по s*/
/* sparseTable по lcp для suffArray*/
/* precalc для sparseTable*/
/* в positions - начала всех вхождений t в s*/
void find(const string& s, const string& t, const vector <int>& suffArray, 
		  const vector < vector <int> >& sparseTable, const vector <int>& precalc, vector <int>& positions) {
	
	/*будем делать 2 бинпоиска: первый для нахождения минимального суффикса, начинающегося с t
								второй для нахождения максимального суффикса, начинающегося с t*/
	int leftBorder, rightBorder;
	/*leftBorder - позиция минимального суффикса, начинающегося с t, в суффмассиве*/
	/*rightBorder - позиция максимального суффикса, начинающегося с t, в суффмассиве*/
	int l = 0;
	int r = suffArray.size() - 1;
	/*mlrLeft - lcp строки t и минимального суффикса*/
	/*mlrRight - lcp строки t и максимального суффикса*/
	int mlrLeft = GetBruteLcp(s, suffArray.front(), t, 0);
	int mlrRight = GetBruteLcp(s, suffArray.back(), t, 0);

	/*первый бинпоиск*/
	while (true) {
		if (mlrLeft == t.size()) {
			/*значит нашли ответ*/
			leftBorder = l;
			break;
		}
		if (l == r) {
			/*значит ни один суффикс не начинается с t*/
			return ;
		}
		if (l == r - 1) {
			if (mlrRight == t.size()) {
				/*значит нашли ответ*/
				leftBorder = r;
				break;
			}
			else {
				/*значит ни один суффикс не начинается с t*/
				return ;
			}
		}

		/*теперь middle != l и middle != r*/
		int middle = (l + r) / 2;
		/*lcpLeftMiddle - lcp суффикса, находящегося на позиции l в suffArray, 
							и суффикса, находящегося на позиции middle в suffArray*/
		/*lcpRightMiddle - lcp суффикса, находящегося на позиции r в suffArray, 
							и суффикса, находящегося на позиции middle в suffArray*/
		int lcpLeftMiddle = GetMin(l, middle, sparseTable, precalc);
		int lcpRightMiddle = GetMin(middle, r, sparseTable, precalc);
		/*mlrMiddle - lcp строки t и суффикса, находящегося на позиции middle в suffArray*/
		int mlrMiddle = max(min(lcpLeftMiddle, mlrLeft), min(lcpRightMiddle, mlrRight));
		/*но, возможно, его можно увеличить*/
		mlrMiddle += GetBruteLcp(s, suffArray[middle] + mlrMiddle, t, mlrMiddle);
		if (mlrMiddle == t.size()) {
			/*тогда ответ может находиться только между l и middle*/
			r = middle;
			mlrRight = mlrMiddle;
			continue;
		}
		
		/*less = true, если суффикс, находящегося на позиции middle в suffArray, меньше строки t*/
		bool less = false;
		if (suffArray[middle] + mlrMiddle == s.size() || s[suffArray[middle] + mlrMiddle] < t[mlrMiddle]) {
			less = true;	
		}
		if (less) {
			/*тогда нужно искать между middle и r*/
			l = middle;
			mlrLeft = mlrMiddle;
		}
		else {
			/*тогда нужно искать между l и middle*/
			r = middle;
			mlrRight = mlrMiddle;
		}
	}	
	
	l = 0;
	r = suffArray.size() - 1;
	mlrLeft = GetBruteLcp(s, suffArray.front(), t, 0);
	mlrRight = GetBruteLcp(s, suffArray.back(), t, 0);
	/*второй бинпоиск*/
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
