#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <ctime>
#include <algorithm>
using namespace std;


int main(int argc, char **argv) {
	srand(time(nullptr));

	ofstream file("data.in");
	int n, e;

	if (argc <= 1) {
		n = rand() % 100;
		e = rand() % ((n * (n - 1)) / 2);
	}
	else if (argc <= 2) {
		n = atoi(argv[1]);
		e = rand() % ((n * (n - 1)) / 2);
	}
	else {
		n = atoi(argv[1]);
		e = atoi(argv[2]);
	}

	vector< pair<int, int> > e1(e), e2(e);

	for (auto &v : e1) {
		v.first = rand() % n;
		v.second = rand() % n;
	}

	if (rand() % 2) {
		for (auto &v : e2) {
			v.first = rand() % n;
			v.second = rand() % n;
		}
	}
	else {
		vector<int> map(n);

		for (int i = 0; i < n; i++) map[i] = i;

		random_shuffle(map.begin(), map.end());

		for (int i = 0; i < e; i++) {
			e2[i].first = map[e1[i].first];
			e2[i].second = map[e1[i].second];
		}

		random_shuffle(e2.begin(), e2.end());
	}

	file << n << ' ' << e << '\n';

	for (auto v : e1) file << v.first << ' ' << v.second << '\n';
	for (auto v : e2) file << v.first << ' ' << v.second << '\n';

	file.close();
}