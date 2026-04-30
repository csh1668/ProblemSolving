#pragma GCC optimize("O3")

#include <bits/stdc++.h>
using namespace std;

int N, M, sx, sy, cnt = 0;
constexpr int MAXN = 2030;
uint8_t vis[MAXN * MAXN];
constexpr int dir[8][2] = {{1, 2}, {1, -2}, {-1, 2}, {-1, -2}, {2, 1}, {2, -1}, {-2, 1}, {-2, -1}};
vector<pair<int, int>> ans;
bool found = false;

uint8_t deg[MAXN * MAXN];

bool in(int x, int y) { return 0 <= x && x < N && 0 <= y && y < M; }
inline int idx(int x, int y) { return x * M + y; }

void decrease_degree(int x, int y) {
  for (int i = 0; i < 8; ++i) {
    int nx = x + dir[i][0], ny = y + dir[i][1];
    if (in(nx, ny)) deg[idx(nx, ny)]--;
  }
}

void increase_degree(int x, int y) {
  for (int i = 0; i < 8; ++i) {
    int nx = x + dir[i][0], ny = y + dir[i][1];
    if (in(nx, ny)) deg[idx(nx, ny)]++;
  }
}

void init() {
  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < M; ++j) {
      for (int k = 0; k < 8; ++k) {
        int nx = i + dir[k][0], ny = j + dir[k][1];
        if (in(nx, ny)) deg[idx(i, j)]++;
      }
    }
  }
}

bool tour(int x, int y) {
  static thread_local mt19937 rng((uint32_t)chrono::high_resolution_clock::now().time_since_epoch().count());

  while (cnt < N * M) {
    int best_deg = INT_MAX;
    int best_nx = -1, best_ny = -1;
    int best_dist2 = -1;
    for (int i = 0; i < 8; ++i) {
      int nx = x + dir[i][0], ny = y + dir[i][1];
      if (!in(nx, ny)) continue;
      int id = idx(nx, ny);
      if (vis[id]) continue;
      int d = deg[id];
      int tx = (nx << 1) - (N - 1);
      int ty = (ny << 1) - (M - 1);
      int dist2 = tx * tx + ty * ty;
      if (d < best_deg) {
        best_deg = d; best_nx = nx; best_ny = ny; best_dist2 = dist2;
      } else if (d == best_deg && best_nx != -1) {
        if (dist2 > best_dist2 || (dist2 == best_dist2 && (rng() & 1) == 0)) {
          best_nx = nx; best_ny = ny; best_dist2 = dist2;
        }
      }
    }
    if (best_nx == -1) break;
    int nid = idx(best_nx, best_ny);
    vis[nid] = 1; cnt++;
    ans.emplace_back(best_nx + 1, best_ny + 1);
    decrease_degree(best_nx, best_ny);
    x = best_nx; y = best_ny;
  }
  return cnt == N * M;
}

int main() {
  ios::sync_with_stdio(false); cin.tie(nullptr);

  cin >> N >> M >> sx >> sy, sx--, sy--;
  
  const int MAX_ATTEMPTS = 6;
  for (int attempt = 0; attempt < MAX_ATTEMPTS && !found; ++attempt) {
    memset(vis, 0, sizeof(vis));
    memset(deg, 0, sizeof(deg));
    ans.clear();
    cnt = 0;
    found = false;

    init();
    vis[idx(sx, sy)] = 1; cnt++;
    ans.emplace_back(sx + 1, sy + 1);
    decrease_degree(sx, sy);
    found = tour(sx, sy);
  }

  if (found) {
    cout << "1\n";
    for (auto [x, y] : ans) cout << x << ' ' << y << '\n';
  } else {
    cout << "0\n";
  }


  return 0;
}