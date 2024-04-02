#pragma once
#include <Siv3D.hpp>

/* 全画像を保存する名前空間 */
namespace images {
	/* 背景画像 */
	typedef struct back {
		back();
		Texture LandScape;
		Texture BackBoard;
		Texture Difficulty;
		Texture ScoreDDD;
		Texture HandUp;
		Texture HandDown;
		Texture HandMaxUp;
		Texture HandMaxDown;
		Texture Pause;
	} back;
	/* タイトル画面の画像の保存先 */
	typedef struct title {
		title();
		Texture Titleimage;
	} title;
	/* チュートリアル画面の画像の保存先 */
	typedef struct tutorial {
		tutorial();
		Texture tutorialimage;
	} tutorial;
	/* エンディング画面の画像の保存先 */
	typedef struct ending {
		ending();
		Texture endingimage;
	} ending;
	/* ブロック画像 */
	typedef struct block {
		block();

		Texture Star;
		Texture Rick;
		Texture Coo;
		Texture Kine;
		Texture Gooey;
		Texture Bomb;
		Texture Hard;
		Texture Twincle;
		Texture none;
	} block;
}
