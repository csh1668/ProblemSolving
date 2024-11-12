#include <bits/stdc++.h>

using namespace std;

// Knuth's Algorithm X
struct node {
    // col: 각 column의 head node
    // l, r, u, d: 좌, 우, 위, 아래 node
    node *col, *l, *r, *u, *d;
    // row: 현재 node의 행 번호
    // size: 현재 column이 가지고 있는 node의 개수
    int row, size;
};

void cover(node* c) {
    // c를 가리키는 양 옆 node를 연결 => 열 제거
    c->r->l = c->l;
    c->l->r = c->r;

    // c열에 속한 모든 노드에 대해서... 아래로 내려가면서
    for (node* i = c->d; i != c; i = i->d) {
        // i행에 속한 모든 노드에 대해서...
        for (node* j = i->r; j != i; j = j->r) {
            // j열을 가리키는 위 아래 node를 연결 => 행 제거
            j->d->u = j->u;
            j->u->d = j->d;
            j->col->size--;
        }
    }
}

void uncover(node* c) {
    // c열에 속한 모든 노드에 대해서... 위로 올라가면서
    for (node* i = c->u; i != c; i = i->u) {
        // i행에 속한 모든 노드에 대해서...
        for (node* j = i->l; j != i; j = j->l) {
            // j열을 가리키는 위 아래 node를 연결 => 행 복구
            j->col->size++;
            j->d->u = j;
            j->u->d = j;
        }
    }

    // c를 가리키는 양 옆 node를 연결 => 열 복구
    c->r->l = c;
    c->l->r = c;
}

bool search(int k, node* head, vector<node*>& answer) {
    if (head->r == head) return true; // 모든 column을 다 cover했을 때
    node* c = nullptr;
    int low = INT_MAX;
    for (node* i = head->r; i != head; i = i->r) {
        if (i->size < low) {
            if (i->size == 0) return false; // 노드가 하나도 없는 column이 존재할 때 => 불가능
            low = i->size;
            c = i;
        }
    }

    cover(c);

    // cover된 column에 속한 모든 노드에 대해서...
    for (node* i = c->d; i != c; i = i->d) {
        answer.push_back(i);
        // 현재 행에 속한 노드들의 head에 대해 cover
        for (node* j = i->r; j != i; j = j->r) {
            cover(j->col);
        }

        if (search(k + 1, head, answer)) {
            return true; // 성공
        } else {
            // 실패
            // 현재 행에 속한 노드들의 head에 대해 uncover
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
    // column head node 생성
    for (int i = 0; i < m; ++i) column[i] = new node();
    for (int i = 0; i < m; ++i) {
        column[i]->col = column[i]->u = column[i]->d = column[i];

        if (i == 0) column[i]->l = head, head->r = column[i];
        else column[i]->l = column[i - 1];
        if (i == m - 1) column[i]->r = head, head->l = column[i];
        else column[i]->r = column[i + 1];
    }
    // matrix에 대한 node 생성
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

    int mp[9][9];
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            cin >> mp[i][j];
        }
    }
    // 729 = 9 * 9 * 9 => 스도쿠의 각 칸에 대한 9가지 가능한 숫자
    // 324 = 9 * 9 + 9 * 9 * 3 => 각 칸에는 하나의 숫자밖에 들어갈 수 없다 + 각 행, 열, 박스에는 중복된 숫자가 들어갈 수 없다
    vector<vector<int>> matrix;
    matrix.reserve(729);

    vector<tuple<int, int, int>> info;
    auto set_row = [&](int i, int j, int k) {
        vector<int> row(324, 0);
        int a = i * 9 + j; // i, j칸을 점유할 때의 column
        int b = i * 9 + k + 81; // i행에 k가 들어갈 때의 column
        int c = j * 9 + k + 162; // j열에 k가 들어갈 때의 column
        int d = (i / 3 * 3 + j / 3) * 9 + k + 243; // i, j칸이 속한 박스에 k가 들어갈 때의 column
        row[a] = row[b] = row[c] = row[d] = 1;
        matrix.push_back(row);
        info.emplace_back(i, j, k);
    };

    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            if (mp[i][j] == 0) {
                for (int k = 0; k < 9; ++k) {
                    set_row(i, j, k);
                }
            } else {
                set_row(i, j, mp[i][j] - 1);
            }
        }
    }

    node* head = build(matrix);
    vector<node*> answer;
    search(0, head, answer);

    for (auto& i : answer) {
        auto& [x, y, k] = info[i->row];
        mp[x][y] = k + 1;
    }

    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            cout << mp[i][j] << ' ';
        }
        cout << '\n';
    }

    return 0;
}