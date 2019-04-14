#include <algorithm>
#include <assert.h>
#include <cstdio>
#include <vector>

using namespace std;

typedef long long i64;

int A(i64 y, int a1, i64 b1, int a2, i64 b2) {
  i64 y1 = a1 * y + b1;
  i64 y2 = a2 * y + b2;
  if (y1 >= y2) {
    return -1;
  }

  /*
   a1 * x + b1 < a2 * x + b2;
   b1 - b2 < (a2 - a1) * x;
   x > (b1 - b2) / (a2 - a1);
   */
  return (b1 - b2) / (a2 - a1) + 1;
}

int B(i64 y, int a1, i64 b1, int a2, i64 b2) {
  i64 y1 = a1 * y + b1;
  i64 y2 = a2 * y + b2;
  if (y1 < y2) {
    return -1;
  }

  /*
   a1 * x + b1 >= a2 * x + b2;
   (a1 - a2) * x >= b2 - b1;
   x >= (b2 - b1) / (a1 - a2);
   */
  return (b2 - b1 + a1 - a2 - 1) / (a1 - a2);
}

typedef int (*func)(i64, int, i64, int, i64);
int m;
void gao(int k, int* w, int* x, int* y, vector<int> & vt) {
  int a1 = m - x[0];
  i64 b1 = x[0];
  int a2 = m - y[0];
  i64 b2 = y[0];
  int a = 1;
  for (int i = 1; i < k; ++i) {
    func f = a ? A : B;
    int nex = f(w[i], a1, b1, a2, b2);
    if (nex != -1) {
      a ^= 1;
      vt.push_back(nex);
      vt.push_back(a ? -1 : 1);
    }
    i64 tmp = w[i];
    a1 -= x[i];
    b1 += tmp * x[i];
    a2 -= y[i];
    b2 += tmp * y[i];
  }
}

int w[110], x[110], y[110];
void solve(int* a, int *b, vector<int> & vt) {
  w[0] = 1;
  x[0] = y[0] = 0;
  int k = 1, i = 0, j = 0;
  while (i < m || j < m) {
    x[k] = y[k] = 0;
    if (i == m) {
      w[k++] = b[j++];
      continue;
    }
    if (j == m) {
      w[k++] = a[i++];
      continue;
    }
    w[k++] = a[i] < b[j] ? a[i++] : b[j++];
  }
  int kk = unique(w, w+k) - w;
  for (i = 0, k = 0; i < m; ++i) {
    while (w[k] < a[i]) {
      k++;
    }
    assert(w[k] == a[i]);
    x[k]++;
  }
  for (j = 0, k = 0; j < m; ++j) {
    while (w[k] < b[j]) {
      k++;
    }
    assert(w[k] == b[j]);
    y[k]++;
  }
  gao(kk, w, x, y, vt);
}

int n;
int s[550][55];
vector<int> vt;
vector<pair<int, int>> tmp;
int main() {
  scanf("%d %d", &n, &m);
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < m; ++j) {
      scanf("%d", &s[i][j]);
    }
    int* a = s[i];
    sort(a, a+m);
  }

  for (int i = 0; i < n; ++i) {
    int* a = s[i];
    tmp.clear();
    for (int j = 0; j < n; ++j) {
      if (i == j) {
        continue;
      }
      int* b = s[j];
      vt.clear();
      solve(a, b, vt);
      for (int k = 0; k < vt.size(); k += 2) {
        tmp.emplace_back(vt[k], vt[k+1]);
      }
    }
    sort(tmp.begin(), tmp.end());
    int nex = tmp.empty() ? 1 : tmp.back().first + 1;
    tmp.emplace_back(nex, 0);
    int cnt = 0;
    int res = 0;
    for (int j = 0; j + 1 < tmp.size(); ++j) {
      cnt += tmp[j].second;
      if (tmp[j].first != tmp[j+1].first) {
        res = max(res, cnt);
      }
    }
    printf("%d\n", n - res);
  }

  return 0;
}
