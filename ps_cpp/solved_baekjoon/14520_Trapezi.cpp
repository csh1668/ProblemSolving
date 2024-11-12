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

int tries = 0;

bool search(int k, node* head, vector<node*>& answer) {
    if (head->r == head) return true; // 모든 column을 다 cover했을 때
    if (tries > 2000000) return false;
    tries++;

    node* c = 0; int low = INT_MAX;
    for (node* i = head->r; i != head; i = i->r) {
        if (i->size < low) {
            if (i->size == 0) 
                return false;
            low = i->size; c = i;
        }
    }

    cover(c);

    for (node* i = c->d; i != c; i = i->d) {
        answer.push_back(i);
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

int main() {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);

#ifndef ONLINE_JUDGE
    string cur = "4d";
    freopen(("trapezi/trapezi." + cur + ".in").c_str(), "r", stdin);
    ifstream ans("trapezi/trapezi." + cur + ".out");
#endif

    int N; cin >> N;
    int max_len = 4 * N - 1;
    vector<vector<int>> mp(N * 2, vector<int>(max_len, -1));
    vector<vector<bool>> is_upper(N * 2, vector<bool>(max_len, false));

    for (int i = 0; i < N * 2; ++i) {
        string s; cin >> s;
        bool flag = i < N;
        int offset = (max_len - s.size()) / 2;
        for (int j = 0; j < s.size(); ++j) {
            if (s[j] == '0') {
                mp[i][offset + j] = 0;
            } else {
                mp[i][offset + j] = -1;
            }
            is_upper[i][offset + j] = flag;
            flag ^= 1;
        }
    }

    // 행 구성 요소
    // rectangle: 각 칸에는 하나의 색깔만 채울 수 있다
    // rectangle * 6 => 각 칸에 대해 6가지 가능한 색깔
    vector<vector<int>> matrix; matrix.reserve(5'000);
    const int rectangle = (N * 2) * max_len;
    // i, j: 시작 좌표
    // k: 색깔
    // d : 방향
    // 0) 수평 방향
    // 1) 아래쪽 -> 오른쪽 (is_upper == true)
    // 2) 오른쪽 -> 위쪽 (is_upper == true)
    // 3) 오른쪽 -> 아래쪽 (is_upper == false)
    // 4) 위쭉 -> 오른쪽 (is_upper == false)
    vector<tuple<int, int, int>> info;

    auto new_row = [&]() -> vector<int>& {
        matrix.emplace_back(rectangle, 0);
        return matrix.back();
    };
    auto coord = [&](int i, int j) -> int {
        return i * max_len + j;
    };
    auto in_range = [&](int i, int j) -> bool {
        return i >= 0 && i < N * 2 && j >= 0 && j < max_len;
    };
    auto is_valid = [&](int i, int j) -> bool {
        return in_range(i, j) && mp[i][j] != -1;
    };
    auto set_row = [&](vector<int>& row, int i, int j) {
        row[coord(i, j)] = 1;
    };

    vector<int>& blank_row = new_row();
    info.emplace_back(-1, -1, -1);

    for (int i = 0; i < N * 2; ++i) {
        for (int j = 0; j < max_len; ++j) {
            if (!is_valid(i, j)) {
                // 아무것도 안채우는 경우
                set_row(blank_row, i, j);
            } else {
                if (is_valid(i, j + 1) && is_valid(i, j + 2)) {
                    auto& row = new_row();
                    set_row(row, i, j); set_row(row, i, j + 1); set_row(row, i, j + 2);
                    info.emplace_back(i, j, 0);
                }
                // 아래쪽 -> 오른쪽
                if (is_upper[i][j] && is_valid(i + 1, j) && is_valid(i + 1, j + 1)) {
                    auto& row = new_row();
                    set_row(row, i, j); set_row(row, i + 1, j); set_row(row, i + 1, j + 1);
                    info.emplace_back(i, j, 1);
                }
                // 오른쪽 -> 위쪽
                if (is_upper[i][j] && is_valid(i, j + 1) && is_valid(i - 1, j + 1)) {
                    auto& row = new_row();
                    set_row(row, i, j); set_row(row, i, j + 1); set_row(row, i - 1, j + 1);
                    info.emplace_back(i, j, 2);
                }
                // 오른쪽 -> 아래쪽
                if (!is_upper[i][j] && is_valid(i, j + 1) && is_valid(i + 1, j + 1)) {
                    auto& row = new_row();
                    set_row(row, i, j); set_row(row, i, j + 1); set_row(row, i + 1, j + 1);
                    info.emplace_back(i, j, 3);
                }
                // 위쪽 -> 오른쪽
                if (!is_upper[i][j] && is_valid(i - 1, j) && is_valid(i - 1, j + 1)) {
                    auto& row = new_row();
                    set_row(row, i, j); set_row(row, i - 1, j); set_row(row, i - 1, j + 1);
                    info.emplace_back(i, j, 4);
                }
            }
        }
    }
#ifndef ONLINE_JUDGE
    cout << "row size: " << matrix[0].size() << endl;
    cout << "matrix size: " << matrix.size() << endl;

    string correct; ans >> correct;
    if (correct == "nemoguce") cout << "correct is nemoguce" << endl;
    else cout << "correct is not nemoguce" << endl;

    for (int col = 0; col < matrix[0].size(); ++col) {
        int acc = 0;
        for (int row = 0; row < matrix.size(); ++row) {
            acc += matrix[row][col];
        }
        if (acc == 0) cout << "column " << col << " is empty" << endl;
    }
    // for (auto& row : matrix) {
    //     for (int i = 0; i < row.size(); ++i) {
    //         cout << row[i] << " ";
    //     }
    //     cout << endl;
    // }
    // cout << "info size: " << info.size() << endl;
    // for (auto& i : info) {
    //     auto [a, b, c, d] = i;
    //     cout << a << " " << b << " " << c << " " << d << endl;
    // }
#endif
    node* head = build(matrix);
    vector<node*> answer;
    bool found = search(0, head, answer);

#ifndef ONLINE_JUDGE
    cout << "answer size: " << answer.size() << endl;
    cout << "tries: " << tries << endl; 
#endif

    if (!found) {
        cout << "nemoguce" << endl;
        return 0;
    }

    const set<int> color_pool = {1, 2, 3, 4, 5, 6};
    set<int> color;
    for (auto& nd : answer) {
        auto& [i, j, d] = info[nd->row];
        if (d == -1) continue;
        color = color_pool;

        auto erase_color = [&](int i, int j) {
            if (is_valid(i, j)) color.erase(mp[i][j]);
        };

        // adjacent한 칸에 같은 색깔이 없도록 제거
        if (d == 0) {
            erase_color(i, j - 1); erase_color(i, j + 3);
            if (is_upper[i][j]) {
                erase_color(i + 1, j); erase_color(i - 1, j + 1); erase_color(i + 1, j + 2);
            } else {
                erase_color(i - 1, j); erase_color(i + 1, j + 1); erase_color(i - 1, j + 2);
            }
            assert(color.size() > 0);

            mp[i][j] = mp[i][j + 1] = mp[i][j + 2] = *color.begin();
        } else if (d == 1) {
            erase_color(i, j - 1); erase_color(i, j + 1);
            erase_color(i + 1, j - 1); erase_color(i + 1, j + 2);
            erase_color(i + 2, j + 1);
            assert(color.size() > 0);

            mp[i][j] = mp[i + 1][j] = mp[i + 1][j + 1] = *color.begin();
        } else if (d == 2) {
            erase_color(i, j - 1); erase_color(i + 1, j);
            erase_color(i, j + 2); erase_color(i - 1, j);
            erase_color(i - 1, j + 2);
            assert(color.size() > 0);

            mp[i][j] = mp[i][j + 1] = mp[i - 1][j + 1] = *color.begin();
        } else if (d == 3) {
            erase_color(i - 1, j); erase_color(i, j - 1);
            erase_color(i, j + 2); erase_color(i + 1, j);
            erase_color(i + 1, j + 2);
            assert(color.size() > 0);

            mp[i][j] = mp[i][j + 1] = mp[i + 1][j + 1] = *color.begin();
        } else if (d == 4) {
            erase_color(i, j - 1); erase_color(i, j + 1);
            erase_color(i - 1, j - 1); erase_color(i - 1, j + 2);
            erase_color(i - 2, j + 1);
            assert(color.size() > 0);

            mp[i][j] = mp[i - 1][j] = mp[i - 1][j + 1] = *color.begin();
        } else assert(0);
    }

    for (int i = 0; i < N * 2; ++i) {
        int offset = i < N ? (N - i - 1) : (i - N);
        for (int j = offset; j < max_len - offset; ++j) {
            if (mp[i][j] == -1) cout << ".";
            else cout << mp[i][j];
        }
        cout << endl;
    }

    return 0;
}