#pragma once
#include "info.h"

namespace Back {
	typedef struct RectColor {
		RectColor();
		/* 表紙する枠の色 */
		Color InnerC;  /* 枠の内側の色 */
		Color OuterC;  /* 枠の外側の色 */
	} RectColor;
	typedef struct Frame {
		Frame();
		/* スコア表示などのオプション */
		uint Lside;  /* スコアなどの枠の左縁のx座標 */
		uint Wide;  /* スコアなどの枠の横幅 */
		uint yHold;  /* ホールド表示の枠の上側のy座標 */
		uint yDifficult;  /* 難易度表示の枠の上側のy座標 */
		uint yNext;  /* 次落ちてくるブロック群を表示する枠の上側のy座標 */
		uint yScore;  /* 現在のスコア表示の枠上側のy座標 */
		uint yCombo;  /* 現在のコンボ数表示枠の上側のy座標 */
		uint yDown;  /* 盤面の底 */
		/* それぞれの枠の定義 */
		Rect RectHold;  /* ホールドの枠 */
		Rect RectDifficult;  /* 難易度表示の枠 */
		Rect RectNext;  /* 次のノード表示の枠 */
		Rect RectScore;  /* スコア表示の枠 */
		Rect RectCombo;  /* 現在のコンボ数表示の枠 */
	} Frame;
}
