#include "UnionFind.h"

UnionFind::UnionFind(uint n) : par(n, 100), rank(n, 1) {}

uint UnionFind::root(uint x) {
	/* xが親の際 */
	if (par[x] == 100) return x;

	/* 経路圧縮 */
	return (par[x] = root(par[x]));
}

bool UnionFind::isSame(uint x, uint y) {
	return (root(x) == root(y));
}

void UnionFind::unite(uint x, uint y) {
	uint rx = root(x), ry = root(y);

	if (rx == ry) return;

	/* ランクを用いて結合の工夫する */
	if (rank[rx] < rank[ry]) std::swap(rx, ry);

	par[rx] = ry;

	rank[rx] += rank[ry];
}
