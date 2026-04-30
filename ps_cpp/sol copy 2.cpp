#pragma GCC optimize("O3")

#include <bits/stdc++.h>
using namespace std;

static const int INF = 1e9;

struct StableRoommates {
  int N;
  vector<deque<int>> L;            // Reduced preference lists (1-indexed)
  vector<vector<int>> rank;        // rank[i][j] = position of j in i's original list

  StableRoommates(int n = 0) { init(n); }

  void init(int n) {
    N = n;
    L.assign(N + 1, {});
    rank.assign(N + 1, vector<int>(N + 1, -1));
  }

  void setPreferences(int i, const vector<int> &lst) {
    L[i] = deque<int>(lst.begin(), lst.end());
    for (int p = 0; p < (int)lst.size(); ++p) rank[i][lst[p]] = p;
  }

  inline bool containsInList(int u, int v) const {
    const auto &dq = L[u];
    for (int x : dq) if (x == v) return true;
    return false;
  }

  inline bool eraseFromList(int u, int v) {
    auto &dq = L[u];
    for (size_t idx = 0; idx < dq.size(); ++idx) if (dq[idx] == v) { dq.erase(dq.begin() + idx); return true; }
    return false;
  }

  int secondChoice(int a) const {
    if ((int)L[a].size() >= 2) return L[a][1];
    return -1;
  }

  int lastChoice(int a) const {
    if (L[a].empty()) return -1;
    return L[a].back();
  }

  bool phase1() {
    queue<int> q; vector<char> inq(N + 1, 0);
    for (int i = 1; i <= N; ++i) { q.push(i); inq[i] = 1; }

    while (!q.empty()) {
      int a = q.front(); q.pop(); inq[a] = 0;
      while (true) {
        if (L[a].empty()) return false;
        int b = L[a].front();

        // If b no longer has a, remove (a,b) and try next
        if (!containsInList(b, a)) {
          eraseFromList(a, b);
          continue;
        }

        // Trim b's tail until last(b) == a
        while (!L[b].empty() && L[b].back() != a) {
          int z = L[b].back();
          L[b].pop_back();
          bool zFrontWasB = (!L[z].empty() && L[z].front() == b);
          eraseFromList(z, b);
          if (L[z].empty()) return false;
          if (zFrontWasB && !inq[z]) { q.push(z); inq[z] = 1; }
        }
        // Done processing a's proposal to b
        break;
      }
    }
    return true;
  }

  bool removeRotationOnce() {
    // try each possible start with at least two options
    for (int start = 1; start <= N; ++start) {
      if ((int)L[start].size() < 2) continue;

      vector<int> A; vector<int> posIn(N + 1, -1);
      int cur = start; bool failed = false;

      while (true) {
        int b = secondChoice(cur);
        if (b == -1) { failed = true; break; }
        int nextA = lastChoice(b);
        if (nextA == -1) { failed = true; break; }

        if (posIn[cur] != -1) {
          int s = posIn[cur];
          vector<int> cyc(A.begin() + s, A.end());
          int K = (int)cyc.size();
          if (K == 0) { failed = true; break; }

          // Snapshot B and next A
          vector<int> B(K), nextAIdx(K);
          for (int i = 0; i < K; ++i) {
            int ai = cyc[i];
            int bi = secondChoice(ai);
            if (bi == -1) { failed = true; break; }
            B[i] = bi;
            nextAIdx[i] = cyc[(i + 1) % K];
          }
          if (failed) break;

          // Delete (ai, bi)
          for (int i = 0; i < K; ++i) {
            int ai = cyc[i], bi = B[i];
            eraseFromList(ai, bi);
            eraseFromList(bi, ai);
            if (L[ai].empty() || L[bi].empty()) { failed = true; break; }
          }
          if (failed) break;

          // For each bi, delete all worse than a_{i+1}
          for (int i = 0; i < K; ++i) {
            int bi = B[i];
            int ai1 = nextAIdx[i];
            // remove from tail while last(bi) != ai1
            while (!L[bi].empty() && L[bi].back() != ai1) {
              int k = L[bi].back();
              L[bi].pop_back();
              eraseFromList(k, bi);
              if (L[bi].empty() || L[k].empty()) { failed = true; break; }
            }
            if (failed) break;
          }
          if (failed) break;

          return true;
        }
        posIn[cur] = (int)A.size();
        A.push_back(cur);
        cur = nextA;
      }
      // try next start
    }
    return false;
  }

  bool solve(vector<pair<int,int>> &teams) {
    if (N % 2 == 1) return false;
    if (!phase1()) return false;

    while (true) {
      // if someone empty -> no solution
      for (int i = 1; i <= N; ++i) if (L[i].empty()) return false;
      bool need = false;
      for (int i = 1; i <= N; ++i) if ((int)L[i].size() >= 2) { need = true; break; }
      if (!need) break;
      if (!removeRotationOnce()) return false;
    }

    vector<int> mate(N + 1, -1); vector<char> used(N + 1, 0);
    for (int i = 1; i <= N; ++i) {
      if ((int)L[i].size() != 1) return false;
      mate[i] = L[i].front();
    }
    for (int i = 1; i <= N; ++i) if (!used[i]) {
      int j = mate[i];
      if (j <= 0) return false;
      teams.push_back({i, j});
      used[i] = used[j] = 1;
    }
    return true;
  }
};

int main() {
  ios::sync_with_stdio(false); cin.tie(nullptr);

  int T; if (!(cin >> T)) return 0;
  while (T--) {
    int N; cin >> N;
    StableRoommates sr(N);
    for (int i = 1; i <= N; ++i) {
      vector<int> lst; lst.reserve(N - 1);
      for (int j = 0; j < N - 1; ++j) {
        int x; cin >> x; lst.push_back(x);
      }
      sr.setPreferences(i, lst);
    }

    vector<pair<int,int>> teams;
    bool ok = sr.solve(teams);
    if (!ok) {
      cout << "NO SOLUTION\n";
      continue;
    }

    // Print each pair once
    vector<char> printed(N + 1, 0);
    bool firstOut = true;
    for (auto &p : teams) {
      int a = p.first, b = p.second;
      if (printed[a] || printed[b]) continue;
      printed[a] = printed[b] = 1;
      if (!firstOut) cout << ' ';
      cout << a << ':' << b;
      firstOut = false;
    }
    cout << '\n';
  }
  return 0;
}