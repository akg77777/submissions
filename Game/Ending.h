#pragma once
#include "info.h"

class Ending : public App::Scene {
	public:
		Ending(const InitData& init);
		void update() override;
		void draw() const override;
	private:
		const images::ending ending_image;
		uint round_score;  /* 今回のゲームスコア */
		bool opened;  /* 今回のスコアが保存されたか */
		/* 毎回スコアを保存するCSVファイルのパスを保存 */
		const String FilePath;
		const CSV file;
		Array<uint> scoredata;
		Rect m_continueButton;  /* コンティニューボタンの枠 */
		Transition m_continueTransition;
		Rect m_endButton;  /* 終了ボタンの枠 */
		Transition m_endTransition;
};

