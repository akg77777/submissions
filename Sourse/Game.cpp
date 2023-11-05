#include "Game.h"

Game::Game(const InitData& init) : IScene{ init } {
	/* フラグなどの初期化を行う */
	Datainit();
	m_pauseButton = Rect{ 5, 5, 70, 70 };
	m_pauseTransition = Transition{ 0s, 0s };
}

void Game::update() {
	if ((m_pauseButton.leftClicked() || KeyEscape.down()) && !process::flag::pausepressed)
		process::flag::pausepressed = !process::flag::pausepressed;

	if (m_pauseButton.mouseOver() && !process::flag::pausepressed)
		Cursor::RequestStyle(CursorStyle::Hand);

	/* ポーズ画面の時は他の操作を一切行わない */
	if (process::flag::pausepressed)
		return;


	if (process::flag::callriseBottom)
		board.riseBottom();

	if (!process::flag::boardinit || process::flag::handsturn) {
		if (!process::flag::boardinit)
			board.init();
		return;
	}

	/* 以下process::flag名前空間内のフラグを用いて遷移し続ける */
	minoadmin.manipulate();  /* minoフェーズ */
	board.arrange();  /* arrangeフェーズ */

	if (!process::flag::gameover)
		process::second::GameOver.reset();

	/* 以下ゲームオーバーの条件 */
	if (process::flag::gameover) {
		changeScene(State::Ending);
	}
}

void Game::draw() const {
	background.draw();  /* 背景画像などの表示 */
	board.draw();  /* 盤面の表示 */

	/* ポーズボタンの表示 */
	if (!process::flag::pausepressed) {
		m_pauseButton.draw(ColorF{ Palette::White }).drawFrame(2);
		FontAsset(U"Menu")(U"=").drawAt(m_pauseButton.center(), ColorF{ 0.25 });
	}
}

void Game::Datainit() {
	process::second::fallsecond = 1.0;
	process::second::GameOver.reset();
	process::flag::boardinit = false;
	process::flag::doarrange = true;
	process::flag::doerase = true;
	process::flag::doexplode = false;
	process::flag::backUpdated = false;
	process::flag::flushing = false;
	process::flag::maketwincle = false;
	process::flag::twincleGenerated = false;
	process::flag::boardrise = false;
	process::flag::handsturn = false;
	process::flag::callriseBottom = false;
	process::flag::minogenerated = true;
	process::flag::calldrop = false;
	process::flag::minowaiting = false;
	process::flag::minocanmove = true;
	process::flag::minoground = false;
	process::flag::Fastpressed = false;
	process::flag::holdgenerated = false;
	process::flag::holdget = false;
	process::flag::Leftrotatepressed = false;
	process::flag::Rightrotatepressed = false;
	process::flag::pausepressed = false;
	process::flag::gameover = false;
	data::score.reset();
	data::turn.reset();
}
