#include <bits/stdc++.h>

using namespace std;

struct node {
    node *col, *l, *r, *u, *d;
    int row, size;
};

void cover(node* c) {
    c->r->l = c->l;
    c->l->r = c->r;
    for (node* i = c->d; i != c; i = i->d) {
        for (node* j = i->r; j != i; j = j->r) {
            j->d->u = j->u;
            j->u->d = j->d;
            j->col->size--;
        }
    }
}

void uncover(node* c) {
    for (node* i = c->u; i != c; i = i->u) {
        for (node* j = i->l; j != i; j = j->l) {
            j->col->size++;
            j->d->u = j;
            j->u->d = j;
        }
    }
    c->r->l = c;
    c->l->r = c;
}

bool search(int k, node* head, vector<int>& answer) {
    if (head->r == head) return true; // 모든 column을 다 cover했을 때

    node* c = 0; int low = INT_MAX;
    for (node* i = head->r; i != head; i = i->r) {
        if (i->size < low) {
            if (i->size == 0) return false;
            low = i->size; c = i;
        }
    }

    cover(c);

    for (node* i = c->d; i != c; i = i->d) {
        answer.push_back(i->row);
        for (node* j = i->r; j != i; j = j->r) {
            cover(j->col);
        }

        if (search(k + 1, head, answer)) {
            return true;
        } else {
            for (node* j = i->l; j != i; j = j->l) {
                uncover(j->col);
            }

            answer.pop_back();
        }
    }

    uncover(c);
    return false;
}

node* build(vector<vector<int>> matrix) {
    int n = matrix.size(), m = matrix[0].size();
    node* head = new node();
    vector<node*> column(m);
    for (int i = 0; i < m; ++i) column[i] = new node();
    for (int i = 0; i < m; ++i) {
        column[i]->col = column[i]->u = column[i]->d = column[i];

        if (i == 0) column[i]->l = head, head->r = column[i];
        else column[i]->l = column[i - 1];
        if (i == m - 1) column[i]->r = head, head->l = column[i];
        else column[i]->r = column[i + 1];
    }

    for (int i = 0; i < n; ++i) {
        node* prev = 0;
        for (int j = 0; j < m; ++j) {
            if (matrix[i][j] == 0) continue;
            node* cur = new node();
            cur->row = i;
            cur->col = column[j];
            cur->u = column[j]->u; cur->d = column[j];
            if (prev) {
                cur->l = prev; cur->r = prev->r;
                prev->r->l = cur; prev->r = cur;
            } else {
                cur->l = cur->r = cur;
            }
            column[j]->u->d = cur; column[j]->u = cur;
            column[j]->size++;
            prev = cur;
        }
    }

    return head;
}

struct pair_hash {
    template <class T1, class T2>
    std::size_t operator () (const std::pair<T1, T2>& p) const {
        auto h1 = std::hash<T1>{}(p.first);
        auto h2 = std::hash<T2>{}(p.second);
        return h1 ^ h2;
    }
};

int main() {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);

    int N, M; cin >> N >> M;
    vector<vector<int>> mp(N, vector<int>(M, 0));
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            cin >> mp[i][j];
        }
    }

    vector<vector<int>> matrix; matrix.reserve(100'000);
    vector<tuple<int, int, int, int>> info;

    const vector<vector<pair<int, int>>> shapes = {
        {{0, 0}, {0, 1}, {-1, 1}, {-1, 2}, {1, 1}}, // F
        {{0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}}, // I
        {{0, 0}, {1, 0}, {2, 0}, {3, 0}, {3, 1}}, // L
        {{0, 0}, {0, 1}, {-1, 1}, {1, 0}, {2, 0}}, // N
        {{0, 0}, {0, 1}, {1, 0}, {1, 1}, {2, 0}}, // P
        {{0, 0}, {0, 1}, {0, 2}, {1, 1}, {2, 1}}, // T
        {{0, 0}, {1, 0}, {1, 1}, {1, 2}, {0, 2}}, // U
        {{0, 0}, {1, 0}, {2, 0}, {2, 1}, {2, 2}}, // V
        {{0, 0}, {1, 0}, {1, 1}, {2, 1}, {2, 2}}, // W
        {{0, 0}, {0, 1}, {0, -1}, {1, 0}, {-1, 0}}, // X
        {{0, 0}, {1, 0}, {2, 0}, {3, 0}, {1, -1}}, // Y
        {{0, 0}, {0, 1}, {1, 1}, {2, 1}, {2, 2}}, // Z
    };

    // 0: 0도
    // 1: 좌우반전
    // 2: 좌우반전 + 상하반전
    // 3: 상하반전
    // 4: 대각선반전 + 90도
    // 5: 대각선반전 + 90도 + 좌우반전
    // 6: 대각선반전 + 90도 + 좌우반전 + 상하반전
    // 7: 대각선반전 + 90도 + 상하반전
    const vector<pair<int, int>> rot = {{1, 1}, {1, -1}, {-1, -1}, {-1, 1}, {1, 1}, {1, -1}, {-1, -1}, {-1, 1}};

    const vector<vector<bool>> rot_info = {
        {1, 1, 1, 1, 1, 1, 1, 1}, // F
        {1, 0, 0, 0, 1, 0, 0, 0}, // I
        {1, 1, 1, 1, 1, 1, 1, 1}, // L
        {1, 1, 1, 1, 1, 1, 1, 1}, // N
        {1, 1, 1, 1, 1, 1, 1, 1}, // P
        {1, 0, 1, 1, 1, 0, 1, 1}, // T
        {1, 1, 1, 1, 1, 1, 1, 1}, // U
        {1, 1, 1, 1, 0, 0, 0, 0}, // V
        {1, 1, 1, 1, 0, 0, 0, 0}, // W
        {1, 0, 0, 0, 0, 0, 0, 0}, // X
        {1, 1, 1, 1, 1, 1, 1, 1}, // Y
        {1, 1, 1, 1, 1, 1, 1, 1}, // Z
    };

    const vector<char> name = {'F', 'I', 'L', 'N', 'P', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};

    const int row_size = 60 + 12;

    unordered_map<pair<int, int>, int, pair_hash> coord_to_idx;

    auto new_row = [&]() -> vector<int>& {
        matrix.emplace_back(row_size, 0);
        return matrix.back();
    };
    auto coord = [&](int i, int j) -> int {
        if (coord_to_idx.find({i, j}) == coord_to_idx.end()) {
            coord_to_idx[{i, j}] = coord_to_idx.size();
        }
        return coord_to_idx[{i, j}];
    };
    auto is_valid = [&](int i, int j) -> bool { return 0 <= i && i < N && 0 <= j && j < M && mp[i][j] == 0; };

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            if (!is_valid(i, j)) {
                // blank_row[coord(i, j)] = 1;
                continue;
            } else {
                for (int k = 0; k < shapes.size(); ++k) {
                    auto& shape = shapes[k];
                    auto& can_rot = rot_info[k];
                    for (int r = 0; r < rot.size(); ++r) {
                        if (!can_rot[r]) continue;
                        auto& [rx, ry] = rot[r];
                        bool can = true;
                        for (auto& [dx, dy] : shape) {
                            int trans_x = dx * rx, trans_y = dy * ry;
                            if (r > 3) swap(trans_x, trans_y);
                            int nx = i + trans_x, ny = j + trans_y;

                            if (!is_valid(nx, ny)) {
                                can = false;
                                break;
                            }
                        }
                        if (!can) continue;
                        auto& row = new_row();
                        for (auto& [dx, dy] : shape) {
                            int trans_x = dx * rx, trans_y = dy * ry;
                            if (r > 3) swap(trans_x, trans_y);
                            int nx = i + trans_x, ny = j + trans_y;

                            row[coord(nx, ny)] = 1;
                        }

                        row[60 + k] = 1;
                        info.emplace_back(i, j, k, r);
                    }
                }
            }
        }
    }

    node* head = build(matrix);
    vector<int> answer;
    bool found = search(0, head, answer);
    assert(found); // 무조건 가능한 입력이 주어진다

    for (auto& nd : answer) {
        auto& [i, j, k, r] = info[nd];
        if (k == -1) continue;

        auto& shape = shapes[k]; auto& [rx, ry] = rot[r];
        for (auto& [dx, dy] : shape) {
            int trans_x = dx * rx, trans_y = dy * ry;
            if (r > 3) swap(trans_x, trans_y);
            int nx = i + trans_x, ny = j + trans_y;

            mp[nx][ny] = name[k];
        }
    }

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            cout << (mp[i][j] == 1 ? '1' : (char)mp[i][j]) << ' ';
        }
        cout << '\n';
    }

#ifndef ONLINE_JUDGE
    cout << "matrix size: " << matrix.size() << endl;
#endif

    return 0;
}