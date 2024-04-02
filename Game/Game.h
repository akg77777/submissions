#pragma once

/* MinoAdmin.hからツリー状に必要なデータが保存 */
#include "MinoAdmin.h"

/* ゲーム画面の管理を行う */
class Game : public App::Scene {
	public:
		Game(const InitData& init);  /* ゲームシーンのコンストラクタ */
		void update() override;
		void draw() const override;
	private:
		void Datainit();  /* ゲーム開始時にデータを初期化する関数 */
		Board board;  /* 盤面を保存する動的オブジェクト */
		BackGround background;  /* 背景を保存するオブジェクト */
		MinoAdmin minoadmin;  /* ミノの管理を行うオブジェクト */
		Rect m_pauseButton;  /* ポーズボタンの枠 */
		Transition m_pauseTransition;
};
