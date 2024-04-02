#pragma once

/* ゲーム画面表示の管理をするクラス */
#include "Block.h"
#include "Back.h"

typedef struct BackGround {
		BackGround();
		void draw() const;
	private:
		/* サイズなどのオプションが保存されたデータメンバ */
		Back::RectColor rectcolor;  /* 色のデータ */
		Back::Frame frame;  /* 枠 */
		images::back back_image;  /* 背景画像の保存先 */
		void drawPicture() const;
		void display() const;  /* スコアやコンボの表示 */
		void ScoreImage() const;  /* スコアの枠に表示するデデデやマスターハンドの処理 */
		void PauseDisplay() const;  /* ポーズ画面の表示 */
		Rect m_continueButton;  /* コンティニューボタンの枠 */
		Transition m_continueTransition;
		Rect m_endButton;  /* ゲーム終了ボタンの枠 */
		Transition m_endTransition;
} BackGround;
