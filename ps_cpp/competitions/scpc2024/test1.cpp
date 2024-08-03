#include <iostream>

using namespace std;

int Answer;
char arr[300000];

int main(int argc, char** argv)
{
    std::ios_base::sync_with_stdio(false); std::cin.tie(NULL);

	int t; cin >> t;
    for (int i = 0; i < t; i++){
        int n, state = 0, cnt = 0; cin >> n;
        cin >> arr;
        char prev = arr[0];
        if (prev == 'A') state = 1;
        else state = -1;

        for (int j = 1; j < n; ++j){
            char cur = arr[j];
            if (cur == 'A'){
                if (state == 1) { // AA
                    cnt += 2;
                } else if (state == 0){ // BA
                    cnt += 1; state = 1;
                } else { // BBA
                    state = 1;
                }
            } else {
                if (state == 1){
                    state = 0;
                } else if (state == 0){
                    state = -1;
                } else {
                    // do nothing
                }
            }
        }

        cout << "Case #" << i + 1 << '\n' << cnt << endl;
    }

	return 0;
}