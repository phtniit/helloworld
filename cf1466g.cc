#include <bits/stdc++.h>
using namespace std;

typedef long long i64;
typedef pair<int, int> pii;

const int maxn = 1000000;
const int maxm = 100000;
//const i64 inf = 998244353;
const i64 inf = 1000000007;

int nex[maxn*2 + 10][26], fail[maxn*2 + 10];
int nn = 0;
void add(char* ss, int len) {
	int p = 0;
	for (int i = 0; i < len; ++i) {
		int c = ss[i] - 'a';
		if (nex[p][c] == 0) {
			nex[p][c] = ++nn;
		}
		p = nex[p][c];
	}
}
int q[maxn*2 + 10];
void get_fail() {
	int hh = 0, tt = 0;
	q[tt++] = 0;
	while (hh < tt) {
		int x = q[hh++];
		for (int i = 0; i < 26; ++i) {
			int y = nex[x][i];
			if (y == 0) {
				continue;
			}
			int fx = fail[x];
			while (fx && nex[fx][i] == 0) {
				fx = fail[fx];
			}
			if (x == 0 || nex[fx][i] == 0) {
				fail[y] = 0;
			} else {
				fail[y] = nex[fx][i];
			}
			q[tt++] = y;
		}
	}
	assert(tt == nn + 1);
}

int cnt[maxn*2 + 10];
void down(char* ss, int len) {
	for (int p = 0, i = 0; i < len; ++i) {
		int c = ss[i] - 'a';
		while (p != 0 && nex[p][c] == 0) {
			p = fail[p];
		}
		p = nex[p][c];
		cnt[p]++;
	}
}
void up() {
	for (int i = nn; i > 0; --i) {
		int ii = q[i];
		cnt[fail[ii]] += cnt[ii];
	}
}

vector<int> vt[maxm + 10];
int ans[maxm + 10];
char r[maxn + 10];
void gao(int k) {
		for (int i = 0; i < vt[k].size(); i += 3) {
		int idx = vt[k][i], offset = vt[k][i+1], lenn = vt[k][i+2];
		char* ss = r + offset;
		int p = 0;
		for (int j = 0; j < lenn; ++j) {
			p = nex[p][ss[j] - 'a'];
		}
		ans[idx] = cnt[p];
	}
}

char t[maxm + 10], s[maxn*2 + 10];
void match(int k, int n, int len, int* prefix) {
	int p = 0;
	for (int i = 0; i < len; ++i) {
		int c = s[i] - 'a';
		while (p != 0 && nex[p][c] == 0) {
			p = fail[p];
		}
		p = nex[p][c];
	}
	memset(cnt, 0, sizeof(cnt));
	int* to_end = cnt;
	for (; p != 0; p = fail[p]) {
		to_end[p] = true;
	}
	for (int i = k; i <= n; ++i) {
		for (int j = 0; j < vt[i].size(); j += 3) {
			int offset = vt[i][j+1], len = vt[i][j+2];
			char* ss = r + offset;
			for (int pp = 0, ii = 0; ii < len; ++ii) {
				int c = ss[ii] - 'a';
				pp = nex[pp][c];
				if (to_end[pp]) {
					prefix[offset+ii]++;
				}
			}
		}
	}
}

int prefix[maxn + 10], suffix[maxn + 10];
int num[26];
int main() {
	int n, m, k, len = 0;
	cin >> n >> m;
	scanf("%s", s);
	scanf("%s", t);

	for (int i = 0; i < m; ++i) {
		scanf("%d %s", &k, &r[len]);
		int lenn = strlen(&r[len]);
		std::reverse(r+len, r+len+lenn);
		add(r+len, lenn);
		std::reverse(r+len, r+len+lenn);
		add(r+len, lenn);

		vt[k].push_back(i);
		vt[k].push_back(len);
		vt[k].push_back(lenn);

		len += lenn;
	}
	get_fail();

	int slen = strlen(s);
	int kk = 0;
	while (kk < n && slen < maxn) {
		memset(cnt, 0, sizeof(cnt));
		down(s, slen);
		up();
		gao(kk);

		s[slen] = t[kk];
		for (int i = 0; i < slen; ++i) {
			s[slen+1+i] = s[i];
		}
		slen = slen * 2 + 1;
		kk++;
	}
	memset(cnt, 0, sizeof(cnt));
	down(s, slen);
	up();
	for (int i = kk; i <= n; ++i) {
		gao(i);
	}

	match(kk+1, n, slen, prefix);

	for (int i = kk + 1; i <= n; ++i) {
		for (int j = 0; j < vt[i].size(); j += 3) {
			int offset = vt[i][j+1], len = vt[i][j+2];
			std::reverse(r + offset, r + offset + len);
		}
	}
	std::reverse(s, s+slen);
	match(kk+1, n, slen, suffix);

	i64 bas = 1;
	for (int i = kk+1; i <= n; ++i) {
		for (int j = 0; j < 26; ++j) {
			num[j] = num[j] * 2 % inf;
		}
		num[t[i-1] - 'a']++;
		bas = bas * 2 % inf;
		for (int j = 0; j < vt[i].size(); j += 3) {
			int idx = vt[i][j], offset = vt[i][j+1], len = vt[i][j+2];
			std::reverse(r + offset, r + offset + len);
			std::reverse(suffix + offset, suffix + offset + len);
			ans[idx] = bas * ans[idx] % inf;
			for (int pos = offset; pos < offset + len; ++pos) {
				if (pos != offset && prefix[pos - 1] == 0) {
					continue;
				}
				if (pos + 1 != offset + len && suffix[pos + 1] == 0) {
					continue;
				}
				ans[idx] = (ans[idx] + num[r[pos] - 'a']) % inf;
			}
		}
	}

	for (int i = 0; i < m; ++i) {
		printf("%d\n", ans[i]);
	}
	return 0;
}
