#pragma once
#include "Mino.h"

/* 中身はMinoAdmin.cppにて定義 */
typedef struct MinoAdmin {  /* Minoクラスを用いてミノの管理を行う名前空間 */
	void manipulate();  /* ミノの管理を全般的に行う関数 */
	void sync();  /* process::minotoboard名前空間とmovingミノの情報を同期させる */
	Mino moving;  /* 落下中のミノ */
	Mino next;  /* 次のターンに呼び出されるミノ */
	Mino hold;  /* ホールド中のミノ */
	Mino before;  /* 毎操作ごとに操作前の状態をここに保存 */
} MinoAdmin;
