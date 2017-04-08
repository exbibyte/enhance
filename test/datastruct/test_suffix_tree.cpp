#include "suffix_tree.hpp"

int main() {
    string text;
    cin >> text;
    vector<string> edges = suffix_tree::ComputeSuffixTreeEdges(text);
    for (int i = 0; i < edges.size(); ++i) {
	// cout << i << ": ";
        cout << edges[i] << endl;
    }
    return 0;
}
