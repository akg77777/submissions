#include "stdafx.h"
#include "images.h"

images::back::back() :
	LandScape(Texture{ U"example/background/BackGround.png" }),  /* 背景画像 */
	BackBoard(Texture{ U"example/background/Board.png" }),  /* 盤面の裏側に表示する画像 */
	Difficulty(Texture{ U"example/background/difficulty.png" }),  /* 難易度表記に用いる画像 */
	ScoreDDD(Texture{ U"example/background/ScoreDDD.png" }),  /* スコア表示の場所に現れるデデデ大王の画像 */
	/* 以下スコア表記の場所に現れるマスター?ハンド */
	HandUp(Texture{ U"example/background/MasterHandUp.png" }),
	HandDown(Texture{ U"example/background/MasterHandDown.png" }),
	HandMaxUp(Texture{ U"example/background/MasterHandMaxup.png" }),
	HandMaxDown(Texture{ U"example/background/MasterHandMaxdown.png" }),
	Pause(Texture{ U"example/background/Pause.png" })
	{ }

images::title::title() : Titleimage(Texture{ U"example/background/Title.png"})
	{}

images::tutorial::tutorial() : tutorialimage(Texture{ U"example/background/Tutorial.png"})
	{}

images::ending::ending() : endingimage(Texture{ U"example/background/Ending.png"})
	{}

images::block::block() :
	Star(Texture{ U"example/block/Star.png" }),
	Rick(Texture{ U"example/block/Rick.png" }),
	Coo(Texture{ U"example/block/Coo.png" }),
	Kine(Texture{ U"example/block/Kine.png" }),
	Gooey(Texture{ U"example/block/Gooey.png" }),
	Bomb(Texture{ U"example/block/Bomb.png" }),
	Hard(Texture{ U"example/block/Hardblock.png" }),
	Twincle(Texture{ U"⭐"_emoji }),
	none(Texture{ U"null" })
	{ }
