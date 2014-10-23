#include <bits/stdc++.h>

using namespace std;

const int MAXN = 5e6;

int S[MAXN], len[MAXN], link[MAXN], to[MAXN][2];

int n, last, sz;

void init()
{
    S [n++] =  2;
    len [1] = -1;
    link[0] =  1;
    sz = 2;
}

int get_link(int v)
{
    while(S[n - len[v] - 2] != S[n - 1]) v = link[v];
    return v;
}

int add_letter(int c)
{
    int ret = 0;
    S[n++] = c;
    int cur = get_link(last);
    if(!to[cur][c])
    {
        len [sz] = len[cur] + 2;
        link[sz] = to[get_link(link[cur])][c];
        to[cur][c] = sz++;
        ret = 1;
    }
    last = to[cur][c];
    return ret;
}

int main()
{
    init();
    char c;
    while((c = getchar()) != '\n')
        putchar('0' + add_letter(c - 'a'));
    return 0;
}
