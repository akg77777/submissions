#include "info.h"

/* グローバル関数などの保存 */

/* arrangeフェーズが終わったらこの関数を呼び出しフラグ等の初期化を行う */
void minoflag() {
	/* 時間周りの設定 */
	if (!process::flag::boardinit) {
		process::second::flushing = 0.8;
		process::second::reflesh = 0.3;
		process::second::betwincle = 0.3;
		process::flag::boardinit = true;
	}
	/* フラグ関係の初期化 */
	process::flag::doarrange = false;
	process::flag::doerase = true;
	process::flag::backUpdated = false;
	process::flag::flushing = false;
	process::flag::maketwincle = false;
	process::flag::twincleGenerated = false;
	process::flag::minogenerated = false;
	process::flag::minocanmove = true;
	process::flag::calldrop = false;
	process::flag::minowaiting = false;
	process::flag::minoground = false;
	process::flag::Fastpressed = false;  /* ミノの操作フェーズに入る前に必ずリセットする */
	process::flag::holdget = false;
	if (process::flag::boardrise) {
		process::flag::boardrise = false;
		process::flag::handsturn = true;
	}
	/* 4ターンに1度マスターハンド?を表示させ盤面のせり上がりを行う */
	if (data::turn.get() % 4 == 2)
		process::flag::boardrise = true;
	/* ターンごとに多少ミノの落下速度も上げる */
	/* 今回は400ターンで落下速度が4倍になるような設定を目指した */
	process::second::fallsecond = process::second::fallsecond - (process::second::fallsecond / 289);
}

/* arrangeフェーズに入る時用のフラグのリセット関数 */
void arrangeflag() {
	process::flag::doarrange = true;
	process::flag::doerase = true;
	process::flag::backUpdated = true;
	process::flag::flushing = false;
	process::flag::maketwincle = false;
	process::flag::twincleGenerated = false;
	process::flag::minogenerated = false;
	process::minotoboard::grounded = false;
	process::minotoboard::vertical = false;
}

/* メルセンヌ・ツイスターの乱数生成器の宣言 */
std::mt19937 mt;

/* メルセンヌツイスターの初期化関数 */
void initMT() {
	std::random_device rnd;
	mt.seed(rnd());
}

double getRand() {
	return std::generate_canonical<double, 100>(mt) * 100;
}
