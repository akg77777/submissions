#pragma once
#include "info.h"

/* 盤面における各ブロックの情報を保存 */

typedef struct Block {
		/* コンストラクタ */
		Block(Blockname b = Blockname::none);
		Block operator=(const Block& block);
		Block operator=(const Blockname& block);
		/* ブロックのTextureを返す */
		Texture address();
		Blockname block;
		/* 点滅させるブロックか保存 */
		bool flush;
		/* きらきらだった星ブロックであればtrue */
		bool twincle;
		/* 落下中のミノを指すブロックならtrue */
		bool mino;
		/* 以下2つはexplosionメンバ関数用のフラグ */
		/* 爆弾ブロックの場所を保存 */
		bool bomb;
	private:
		images::block block_image;
} Block;

