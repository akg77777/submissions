#pragma once

#include "Board.h"

/* ミノの簡単な操作とデータの保存を行う */

typedef struct Mino {
	Mino();  /* コンストラクタ */
	Mino(Blockname bn1, Blockname bn2);
	Mino operator=(const Mino& mino);
	Blockname block1;  /* 左または上にあるブロック */
	Blockname block2;  /* 右または下にあるブロック */
	bool vertical;  /* ミノの状態が垂直方向であればtrue */
	uint x;  /* block1のx座標 */
	uint y;  /* block2のy座標 */
} Mino;
