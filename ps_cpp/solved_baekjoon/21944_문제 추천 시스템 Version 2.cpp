#include <bits/stdc++.h>

using namespace std;

struct Problem { int P, L, G; };

auto sort_by_P = [](const Problem& a, const Problem& b) { return a.P < b.P; };
auto sort_by_L = [](const Problem& a, const Problem& b) { if (a.L == b.L) return a.P < b.P; return a.L < b.L; };

set<Problem, decltype(sort_by_P)> byP;
set<Problem, decltype(sort_by_L)> byG[101];
set<Problem, decltype(sort_by_L)> byL;

template<typename T>
void debug(T s) {
    for (auto x : s) {
        cout << "(" << x.P << ", " << x.L << ", " << x.G << ") ";
    }
    cout << '\n';
}

void add_problem(int P, int L, int G) {
    byP.insert({P, L, G});
    byG[G].insert({P, L, G});
    byL.insert({P, L, G});
}

void remove_problem(int P) {
    auto problem = *byP.lower_bound({P, 0, 0});
    assert(problem.P == P);
    byP.erase(problem);
    byG[problem.G].erase(problem);
    byL.erase(problem);
}

int recommend_problem(int G, int op) {
    if (op == 1) {
        return byG[G].rbegin()->P;
    } else {
        return byG[G].begin()->P;
    }
}

int recommend_problem2(int op) {
    if (op == 1) {
        return byL.rbegin()->P;
    } else {
        return byL.begin()->P;
    }
}

int recommend_problem3(int L, int op) {
    if (op == 1) {
        auto it = byL.lower_bound({0, L, 0});
        if (it == byL.end()) return -1;
        return it->P;
    } else {
        auto it = byL.lower_bound({0, L, 0});
        if (it == byL.begin()) return -1;
        return (--it)->P;
    }
}

int main() {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);

    int N, M; cin >> N;

    for (int i = 0; i < N; ++i) {
        int P, L, G; cin >> P >> L; G = 0;
        add_problem(P, L, G);
    }

    cin >> M;
    while (M--) {
        string cmd; cin >> cmd;
        if (cmd[0] == 'a') { // add
            int P, L, G; cin >> P >> L; G = 0;
            add_problem(P, L, G);
        } else if (cmd[0] == 's') { // solved
            int P; cin >> P;
            remove_problem(P);
        } else if (cmd.back() == 'd') { // recommend
            int G, op; cin >> op;
            cout << recommend_problem2(op) << '\n';
        } 
        // else if (cmd.back() == '2') { // recommend2
        //     int op; cin >> op;
        //     cout << recommend_problem2(op) << '\n';
        // } else { // recommend3
        //     int op, L; cin >> op >> L;
        //     cout << recommend_problem3(L, op) << '\n';
        // }
    }

    return 0;
}