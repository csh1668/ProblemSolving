#include <bits/stdc++.h>

using namespace std;
typedef long long int ll;

struct UndirectedEdge {
    int u, v;
    UndirectedEdge(int u, int v) : u(min(u, v)), v(max(u, v)) {}
    bool operator<(const UndirectedEdge& other) const {
        return tie(u, v) < tie(other.u, other.v);
    }
};

struct Node {
    Node *l, *r, *p;
    int cnt;

    ll v, sum, lmx, rmx, mx;

    int vertex_id;
    UndirectedEdge edge_id;
    Node(ll v = 0, Node *p = 0) : l(0), r(0), p(p), cnt(1), v(v), sum(v), lmx(v), rmx(v), mx(v), vertex_id(-1), edge_id(-1, -1) {}
    void push() {
        // Do nothing
    }
    void pull() {
        cnt = 1; sum = v; lmx = rmx = mx = v;
        if (l && r) {
            l->push(); r->push();
            cnt += l->cnt + r->cnt;
            sum += l->sum + r->sum;
            lmx = max({l->lmx, l->sum + v, l->sum + v + r->lmx});
            rmx = max({r->rmx, r->sum + v, r->sum + v + l->rmx});
            mx = max({l->mx, r->mx, l->rmx + v + r->lmx, l->rmx + v, v + r->lmx, mx});
        }
        else if (l) {
            l->push();
            cnt += l->cnt;
            sum += l->sum;
            lmx = max(l->lmx, l->sum + v);
            rmx = max(l->rmx + v, v);
            mx = max({mx, l->mx, l->rmx + v});
        }
        else if (r) {
            r->push();
            cnt += r->cnt;
            sum += r->sum;
            rmx = max(r->rmx, r->sum + v);
            lmx = max(r->lmx + v, v);
            mx = max({mx, r->mx, v + r->lmx});
        }
    }
};
typedef Node* pNode;

void rotate(pNode x) {
    pNode p = x->p, y = 0;
    if (!p) return;
    p->push(); x->push();
    if (x == p->l) {
        p->l = y = x->r;
        x->r = p;
    } else {
        p->r = y = x->l;
        x->l = p;
    }
    x->p = p->p; p->p = x;
    if (y) y->p = p;
    if (x->p) {
        if (x->p->l == p) {
            x->p->l = x;
        } else {
            x->p->r = x;
        }
    } else {
        // x is the new root
    }
    p->pull(); x->pull();
}

void splay(pNode x) {
    while (x->p) {
        pNode p = x->p, g = p->p;
        if (g) {
            if ((x == p->l) == (p == g->l)) rotate(p);
            else rotate(x);
        }
        rotate(x);
    }
}

pNode find(pNode x, int k) { // 1-based
    if (!x || k <= 0 || k > x->cnt) return 0;
    x->push();
    int lcnt = x->l ? x->l->cnt : 0;
    if (k <= lcnt) return find(x->l, k);
    else if (k == lcnt + 1) {
        splay(x);
        return x;
    } else {
        return find(x->r, k - lcnt - 1);
    }
}

int index(pNode x) { // 1-based
    if (!x) return 0;
    x->push();
    int lcnt = x->l ? x->l->cnt : 0;
    return lcnt + 1 + index(x->p);
}

pNode first(pNode x) {
    if (!x) return 0;
    x->push();
    while (x->l) {
        x = x->l;
        x->push();
    }
    splay(x);
    return x;
}

pNode last(pNode x) {
    if (!x) return 0;
    x->push();
    while (x->r) {
        x = x->r;
        x->push();
    }
    splay(x);
    return x;
}

pNode root(pNode x) {
    if (!x) return 0;
    while (x->p) {
        x = x->p;
        x->push();
    }
    return x;
}

// (A, B): first k nodes, remaining nodes
pair<pNode, pNode> split(pNode x, int k) { // 1-based
    if (!x || k <= 0) return {0, x};
    if (k >= x->cnt) {
        return {x, 0};
    } 

    pNode L = find(x, k), R = L->r;
    if (R) R->p = 0;
    L->r = 0; L->pull();
    return {L, R};
}

// first to x->cnt nodes, remaining nodes
// returns remaining nodes
pNode split(pNode x) {
    if (!x) return 0;
    int ind = index(x);
    auto [A, B] = split(x, ind);
    return B;
}

pNode merge(pNode l, pNode r) {
    if (!l || !r) return l ? l : r;
    l = find(l, l->cnt);
    l->r = r; r->p = l;
    l->pull();
    return l;
}

void update(pNode x, ll v) {
    if (!x) return;
    splay(x);
    x->v = v;
    x->pull();
}

struct ETT {
  vector<pNode> vertices;
  map<UndirectedEdge, pair<pNode, pNode>> edges;
  
  ETT(int num_v) {
    for (int i = 0; i < num_v; ++i) {
        pNode new_node = new Node(0);
        new_node->vertex_id = i;
        vertices.push_back(new_node);
    }
  }

  bool connected(int u, int v) {
    return root(vertices[u]) == root(vertices[v]);
  }

  void reroot(int u) {
    auto u_node = vertices[u];
    splay(u_node);
    int ind_u = index(u_node);

    auto [A, BC] = split(u_node, ind_u - 1);
    auto [B, C] = split(BC, 1);

    assert(B->vertex_id == u);

    pNode v_edge = last(A);
    if (v_edge) {
        auto [x, y] = v_edge->edge_id;
        int v = (x == u) ? y : x;

        auto [forward, backward] = edges[UndirectedEdge(v, u)];

        ll weight = abs(forward->v);
        assert(abs(forward->v) == abs(backward->v));

        if (forward->v > 0) {
            update(forward, -weight);
            update(backward, weight);
        } else {
            update(forward, weight);
            update(backward, -weight);
        }

        // edges[UndirectedEdge(v, u)] = {backward, forward};
    }

    merge(B, merge(C, A));
    splay(u_node);
  }

  void add_edge(int u, int v, ll w) {
    assert(connected(u, v) == false);

    UndirectedEdge e(u, v);
    tie(u, v) = make_pair(min(u, v), max(u, v));
    reroot(u); reroot(v);

    pNode forward = new Node(w);
    pNode backward = new Node(-w);
    pNode u_node = vertices[u];
    pNode v_node = vertices[v];

    forward->edge_id = e;
    backward->edge_id = e;

    edges[e] = {forward, backward};

    merge(u_node, merge(forward, merge(v_node, backward)));
  }

  void remove_edge(int u, int v) {
    assert(connected(u, v) == true);

    UndirectedEdge e(u, v);
    tie(u, v) = make_pair(min(u, v), max(u, v));

    auto [forward, backward] = edges[e];
    if (!forward || !backward) return;
    edges.erase(e);

    auto [uv_pref, uv_suff_and_uv] = split(forward, index(forward) - 1);
    auto [uv, uv_suff] = split(uv_suff_and_uv, 1);

    auto [vu_pref, vu_suff_and_vu] = split(backward, index(backward) - 1);
    auto [vu, vu_suff] = split(vu_suff_and_vu, 1);

    if (root(uv_suff) == root(vu_pref)) {
        merge(uv_pref, vu_suff);
    } else {
        assert(root(uv_pref) == root(vu_suff));
        merge(vu_pref, uv_suff);
    }

    delete forward;
    delete backward;
  }
};

void debug(pNode x, int depth = 0) {
    if (!x) return;
    debug(x->l, depth + 1);
    cout << string(depth * 2, ' ');
    cout << "Node: " << x->v << ", cnt: " << x->cnt << ", sum: " << x->sum
         << ", lmx: " << x->lmx << ", rmx: " << x->rmx << ", mx: " << x->mx << ", ";
    if (x->edge_id.u != -1) {
        cout << "edge_id: (" << x->edge_id.u << ", " << x->edge_id.v << ")\n";
    } else {
        cout << "vertex_id: " << x->vertex_id << "\n";
    }
        //  << ", vertex_id: " << x->vertex_id
        //  << ", edge_id: (" << x->edge_id.u << ", " << x->edge_id.v << ")\n";
    debug(x->r, depth + 1);
}

int main() {
    ios::sync_with_stdio(0); cin.tie(0); cout.tie(0);

    int N, Q; cin >> N >> Q;
    ETT ett(N + 1);

    for (int i = 0; i < N - 1; ++i) {
        int u, v, w; cin >> u >> v >> w;
        ett.add_edge(u, v, w);
    }

    ett.reroot(5);
    debug(ett.vertices[5]);

    return 0;
}