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

bool search(int k, node* head, vector<node*>& answer) {
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

    while (1) {
        string s; cin >> s;
        if (s == "end") break;
        vector<vector<int>> mp(9, vector<int>(9));

        for (int i = 0; i < 9; ++i) {
            for (int j = 0; j < 9; ++j) {
                if (s[i * 9 + j] == '.') mp[i][j] = 0;
                else mp[i][j] = s[i * 9 + j] - '0';
            }
        }

        vector<vector<int>> matrix; matrix.reserve(9 * 9 * 9);
        vector<tuple<int, int, int>> info;
        auto push_row = [&](int i, int j, int k) {
            matrix.push_back(vector<int>(9 * 9 * 4));
            auto& row = matrix.back();
            row[i * 9 + j] = row[81 + i * 9 + k] = row[81 * 2 + j * 9 + k] = row[81 * 3 + (i / 3 * 3 + j / 3) * 9 + k] = 1;
            info.emplace_back(i, j, k);
        };

        for (int i = 0; i < 9; ++i) {
            for (int j = 0; j < 9; ++j) {
                if (mp[i][j] == 0) {
                    for (int k = 0; k < 9; ++k) {
                        push_row(i, j, k);
                    }
                } else {
                    push_row(i, j, mp[i][j] - 1);
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
                cout << mp[i][j];
            }
        } cout << '\n';

        for (auto& i : matrix) i.clear();
        matrix.clear();
        info.clear();

        for (node* i = head->r; i != head; i = i->r) {
            for (node* j = i->d; j != i; j = j->d) {
                delete j;
            }
            delete i;
        }

        delete head;
    }

    return 0;
}