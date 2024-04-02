#pragma once

/* Board構造体のメンバ関数であるerase関数の処理に用いる */

/* Union-Find木の実装 */
typedef struct UnionFind {
	/* 親の要素と各要素のランク管理を行う変数 */
	/* parの中身はpar[y * COL + x]のように1次元配列として座標を扱う */
	Array<uint> par, rank;

	/* コンストラクタ */
	UnionFind(uint n);

	/* 座標xの親を返す */
	uint root(uint x);

	/* 座標a, bが同じグループに属しているか */
	bool isSame(uint a, uint b);

	/* グループの結合 */
	void unite(uint a, uint b);
} UnionFind;

