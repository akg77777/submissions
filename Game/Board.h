#pragma once

/* 盤面を保存する構造体Boardの定義を行うヘッダ */

#include "BackGround.h"
#include "UnionFind.h"

typedef struct Board {
		/* publicなメンバ関数 */
		Board();  /* コンストラクタ */
		~Board();  /* デストラクタ */
		void init();  /* 最初の盤面をランダムに生成する関数 */
		void arrange();  /* erase関数とdrop関数をまとめる */
		void draw() const;  /* 盤面を表示 */
		void fillRandBoard();  /* 盤面をランダムにブロックですべて埋める */
		void riseBottom();  /* 盤面の底1列分ランダムに追加する関数 */
		bool inSafe();  /* これがfalseの時ゲームオーバー */
	private:
		/* メンバデータ */
		Block** map;  /* 盤面を保存する変数 */
		mutable bool updated;  /* 盤面が更新されたらfalseに変更される */
		bool boardfilled;  /* fillRandBoard()関数が呼ばれたか */
		bool flushflag;  /* 点滅フラグ */
		bool explodeflag;  /* 爆発フラグ */
		uint exploded;  /* 爆発した個数を保存 */
		Stopwatch flush_time;  /* 点滅してから経過した秒数の保存 */
		Stopwatch reflesh_time;  /* drop関数が用いられた後の画面の停止時間 */
		/* privateなメンバ関数 */
		void flagreset();  /* その他データの初期化を行う関数 */
		void judgeMino();  /* ミノを指定通りに移動できるか判定 */
		void PopMino();  /* ミノの削除 */
		void MinoInsert();  /* 落下中のミノをmapに座標とともに保存してdraw関数で表示させる */
		void isGrounded();  /* ミノが設置しているか判定を行う関数 */
		void erase();  /*消せるブロックを削除 */
		void explosion();  /* 爆弾ブロックの処理 */
		void drop();  /* 浮いているブロックをすべて落とす */
		uint getTop(uint x);  /* その列で最も上にあるブロックの場所を返す */
		void insert(Blockname newblock, uint x);  /* 指定したx座標の場所の上に一番上の場所にブロック追加 */
		Blockname genRandBlock();  /* 確率を指定してブロックを生成する関数 */
		Blockname genRandBlock(Array<Blockname> ar);  /* riseBottom関数用のランダムな生成関数 */
		/* 以下きらきらの処理を行う関数群 */
		void comboProcess();  /* コンボ数ときらきらの生成及び削除を行う */
		void genTwincle();  /* コンボ数に対応したきらきらの生成 */
		void toStar();  /* 地に着いたきらきらを星ブロックに変換 */
		void popTwincle();  /* 生成されたキラキラをまとめて削除する関数 */
} Board;
