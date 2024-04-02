#pragma once

#include "images.h"

using uint = uint16;  /* 基本の型 */

/* data名前空間のscore, combo, turn変数用クラス */
struct Data {
		Data();  /* コンストラクタ */

		/* 演算子のオーバーロード */
		void operator++(int d);
		Data operator+(const uint& v);
		Data operator=(const uint& num);

		void reset();  /* valueのリセット */

		uint get();  /* valueを返す */

		/* (combo変数用)生成するきらきら(twincle)の数を返す */
		uint getTwincle();

	private:
		uint value;
};

/* 現在のコンボ数・スコア・ターン数の保存 */
namespace data {
	extern Data combo;
	extern Data score;
	extern Data turn;
}
