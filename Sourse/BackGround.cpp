#include "BackGround.h"

BackGround::BackGround() {
	m_continueButton = Rect{ 290, 230, 400, 100 };
	m_continueTransition = Transition{ 0s, 0s };
	m_endButton = Rect{ 290, 370, 430, 100 };
	m_endTransition = Transition{ 0s, 0s };
}

void BackGround::draw() const {
	if (process::flag::pausepressed) {
		PauseDisplay();
		if (m_continueButton.leftClicked())
			process::flag::pausepressed = false;
		if (m_endButton.leftClicked())
			System::Exit();
		if (m_continueButton.mouseOver() || m_endButton.mouseOver())
			Cursor::RequestStyle(CursorStyle::Hand);
		return;
	}

	drawPicture();
	display();
}

void BackGround::drawPicture() const {
	/* 背景画像の描画 */
	back_image.LandScape.resized(width::window::v, width::window::h).draw(0, 0);
	/* 盤面の背景 */
	back_image.BackBoard.resized(width::board::H, width::board::V)
		.draw(width::outer::L, width::window::h - (width::outer::UD + width::board::V));
	display();
}

void BackGround::display() const {  /* 現在のスコアとコンボ数などの表示 */
	static String Score, Combo;
	static Vec2 ScorePosition, ComboPosition;
	static Font fontScore, fontCombo;
	static Texture NextBlock1, NextBlock2;
	static Texture HoldBlock1, HoldBlock2;
	constexpr uint SIZE = 150;  /* ホールドとネクストに表示するブロックのサイズ */

	if (!process::flag::backUpdated && !process::flag::flushing) {
		/* 現在のスコアとコンボ数の読み込み */
		Score = Format(data::score.get());
		Combo = Format(data::combo.get());
		float scaleScore = 1.0f / static_cast<float>(Score.length());
		float scaleCombo = 1.0f / static_cast<float>(Combo.length());
		/* スコアとコンボ数の表示場所の設定 */
		/* スコア表示の場所には上2/3はデデデが占めており、下1/3には実際のスコアを表示するようにする */
		ScorePosition = Vec2(frame.Lside + (frame.Wide / 2), (frame.yScore * 1.0f / 6 + frame.yCombo * 5.0f / 6));
		ComboPosition = Vec2(frame.Lside + (frame.Wide / 2), frame.yCombo / 2 + frame.yDown / 2);
		/* フォントの設定 */
		fontScore = Font(40.0 * scaleScore, Typeface::Icon_Awesome_Brand);
		fontCombo = Font(40.0 * scaleCombo, Typeface::Icon_Awesome_Brand);
		Block Next1 = process::minotoback::nextblock1;
		Block Next2 = process::minotoback::nextblock2;
		NextBlock1 = Next1.address();
		NextBlock2 = Next2.address();
		if (process::flag::holdgenerated) {
			Block Hold1 = process::minotoback::holdblock1;
			Block Hold2 = process::minotoback::holdblock2;
			HoldBlock1 = Hold1.address();
			HoldBlock2 = Hold2.address();
		}
		process::flag::backUpdated = true;  /* 読み込んだらリセット */
	}

	/* 内容と枠の表示 */
	frame.RectHold.draw(rectcolor.InnerC).drawFrame(0.5, 0.5, rectcolor.OuterC);
	if (process::flag::holdgenerated) {
		HoldBlock1.resized(SIZE, SIZE).draw(frame.Lside, frame.yHold);
		HoldBlock2.resized(SIZE, SIZE).draw(frame.Lside + SIZE, frame.yHold);
	}

	frame.RectDifficult.draw(rectcolor.InnerC).drawFrame(0.5, 0.5, rectcolor.OuterC);

	/* 難易度表記の画像添付 */
	back_image.Difficulty.resized(frame.Wide, frame.yHold - frame.yDifficult)
		.draw(frame.Lside, frame.yDifficult).drawFrame(0.5, 0.5, rectcolor.OuterC);

	frame.RectNext.draw(rectcolor.InnerC).drawFrame(0.5, 0.5, rectcolor.OuterC);

	if (process::flag::boardinit) {
		NextBlock1.resized(SIZE, SIZE).draw(frame.Lside, frame.yNext);
		NextBlock2.resized(SIZE, SIZE).draw(frame.Lside + SIZE, frame.yNext);
	}

	ScoreImage();  /* スコア枠の表示 */

	frame.RectCombo.draw(rectcolor.InnerC).drawFrame(0.5, 0.5, rectcolor.OuterC);

	/* スコア・コンボの表示 */
	ClearPrint();
	fontScore(Score).drawAt(ScorePosition, Palette::Black);
	if(data::combo.get() > 1)
		fontCombo(Combo).drawAt(ComboPosition, Palette::Black);
}

void BackGround::ScoreImage() const {
	const double T = Periodic::Sawtooth1_1(0.15s);  /* マスターハンド?の表示の際に用いる */

	/* スコア表示枠の内容 */
	frame.RectScore.draw(rectcolor.InnerC).drawFrame(0.5, 0.5, rectcolor.OuterC);

	/* 以下実際に盤面をせり上げる直前のマスターハンド?の動きを再現(GB版カービィのきらきらきっず) */
	if (process::flag::handsturn) {
		static int phase = 0;
		static Stopwatch stopwatch{ StartImmediately::Yes };

		if (phase == 0 && stopwatch.sF() > 0.05) {
			phase++;
			stopwatch.reset();
		}
		else if (phase == 0) {
			back_image.HandUp.resized(frame.Wide, (frame.yCombo - frame.yScore) * (2.0f / 3))
				.draw(frame.Lside, frame.yScore).drawFrame(0.5, 0.5, rectcolor.OuterC);
			if (!stopwatch.isStarted())
				stopwatch.start();
		}
		if (phase == 1 && stopwatch.sF() > 0.05) {
			phase++;
			stopwatch.reset();
		}
		else if (phase == 1) {
			back_image.HandMaxUp.resized(frame.Wide, (frame.yCombo - frame.yScore) * (2.0f / 3))
				.draw(frame.Lside, frame.yScore).drawFrame(0.5, 0.5, rectcolor.OuterC);
			if (!stopwatch.isStarted())
				stopwatch.restart();
		}
		if (phase == 2 && stopwatch.sF() > 0.02) {
			phase++;
			stopwatch.reset();
		}
		else if (phase == 2) {
			back_image.HandDown.resized(frame.Wide, (frame.yCombo - frame.yScore) * (2.0f / 3))
				.draw(frame.Lside, frame.yScore).drawFrame(0.5, 0.5, rectcolor.OuterC);
			if (!stopwatch.isStarted())
				stopwatch.restart();
		}
		if (phase == 3 && stopwatch.sF() > 0.1) {
			phase++;
			stopwatch.reset();
		}
		else if (phase == 3) {
			back_image.HandMaxDown.resized(frame.Wide, (frame.yCombo - frame.yScore) * (2.0f / 3))
				.draw(frame.Lside, frame.yScore).drawFrame(0.5, 0.5, rectcolor.OuterC);
			if (!stopwatch.isStarted())
				stopwatch.restart();
		}
		if (phase == 4) {
			back_image.HandDown.resized(frame.Wide, (frame.yCombo - frame.yScore) * (2.0f / 3))
				.draw(frame.Lside, frame.yScore).drawFrame(0.5, 0.5, rectcolor.OuterC);
			if (!stopwatch.isStarted())
				stopwatch.restart();
		}
		if (phase == 4 && stopwatch.sF() > 0.01) {
			phase = 0;
			stopwatch.reset();
			process::flag::boardrise = false;
			process::flag::callriseBottom = true;
			process::flag::handsturn = false;
		}
	}
	/* 通常はデデデを表示させる */
	else if (!process::flag::boardrise) {
		back_image.ScoreDDD.resized(frame.Wide, (frame.yCombo - frame.yScore) * (2.0f / 3))
			.draw(frame.Lside, frame.yScore).drawFrame(0.5, 0.5, rectcolor.OuterC);
	}
	/* sマスターハンド?を表示させる */
	else if (process::flag::boardrise) {  /* 盤面がせりあがる直前 */
		/* マスターハンドが震えているように演出 */
		if (T > 0) {
			back_image.HandUp.resized(frame.Wide, (frame.yCombo - frame.yScore) * (2.0f / 3))
				.draw(frame.Lside, frame.yScore).drawFrame(0.5, 0.5, rectcolor.OuterC);
		}
		else {
			back_image.HandDown.resized(frame.Wide, (frame.yCombo - frame.yScore) * (2.0f / 3))
				.draw(frame.Lside, frame.yScore).drawFrame(0.5, 0.5, rectcolor.OuterC);
		}
	}
}

void BackGround::PauseDisplay() const {
	/* 背景の表示 */
	back_image.Pause.resized(width::window::h, width::window::v).draw(0, 0);

	/* ボタンの表示(透明) */
	m_continueButton.draw(ColorF{ 0, 0, 0, 0 });
	m_endButton.draw(ColorF{ 0, 0, 0, 0 });
}
