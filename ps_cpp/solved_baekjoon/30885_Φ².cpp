#include <bits/stdc++.h>

using namespace std;
typedef long long int ll;

struct Node {
    ll v; int pos;
    Node *prev, *next;
    Node(ll v, int pos) : v(v), pos(pos), prev(0), next(0) {}
};

int main() {
    ios::sync_with_stdio(0); cin.tie(0); cout.tie(0);

    int N; cin >> N;
    vector<Node*> nodes(N + 1);
    Node* head = (nodes[0] = new Node(LLONG_MAX, 0));
    for (int i = 1; i <= N; ++i) {
        ll v; cin >> v;
        nodes[i] = new Node(v, i);
        if (i) {
            nodes[i]->prev = nodes[i - 1];
            nodes[i - 1]->next = nodes[i];
        }
    }

    while (head->next->next) {
        Node* cur = head->next;
        while (cur) {
            ll v = cur->v, prev_v = cur->prev->v, next_v = cur->next ? cur->next->v : LLONG_MAX;
            if (prev_v <= v) {
                Node* to_delete = cur->prev;
                to_delete->prev->next = cur;
                cur->prev = to_delete->prev;
                cur->v += prev_v;
            }
            if (next_v <= v) {
                Node* to_delete = cur->next;
                cur->next = to_delete->next;
                if (to_delete->next) {
                    to_delete->next->prev = cur;
                }
                cur->v += next_v;
            }
            cur = cur->next;
        }
    }

    cout << head->next->v << '\n' << head->next->pos << '\n';

    return 0;
}