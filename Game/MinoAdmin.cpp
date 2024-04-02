#include "MinoAdmin.h"

/* ミノの操作は全てこれで管理(ミノの生成から盤面への設置まで) */

void MinoAdmin::manipulate() {
	if (process::flag::gameover) {
		return;
	}

	static Stopwatch falling;
	double fall_second;

	/* ブロック2のy座標を返す関数 */
	auto block2y = [](int block1y, bool vertical) {
			if (vertical)
				return block1y - 1;
			else
				return block1y;
		};

	if (!process::flag::doarrange && !process::flag::minoground) {  /* ミノの操作フェーズの時のみこの処理を実行 */
		if (!process::flag::minogenerated) {  /* 各ターン最初のみミノを生成 */
			if (DEBUG) Logger << U"minogenerated";
			if (data::turn.get() > 1) {  /* 2ターン目以降 */
				if (DEBUG) Logger << U"turn" << data::turn.get();
				/* もうnextにミノはあるはずなのでそれを持ってきてnextは新しいものをランダム生成 */
				moving = next;
				next = Mino();  /* 更新 */
			}
			process::flag::minogenerated = true;
			falling.restart();
			sync();
			process::flag::backUpdated = false;
		}
		/* もし前回呼ばれた際に実行された処理がBoardオブジェクトに拒否されたら */
		if (!process::flag::minocanmove) {
			/* 前回の状態に戻す */
			moving = before;
		}

		/* 以下ミノを生成後 */
		/* このmanipulate関数が呼ばれるたびに一回のみ操作をし、次の操作の反映はまた呼ばれたときにするようにする */

		if (process::flag::Fastpressed)  /* フラグにより落下速度を変化 */
			fall_second = process::second::fallsecond / 5;
		else
			fall_second = process::second::fallsecond;


		/* Fastキーが押されている際2倍速度が上がり離されたら元に戻る */
		if (input::Fast.down()) {
			process::flag::Fastpressed = true;
		}
		else if (input::Fast.up()) {
			process::flag::Fastpressed = false;
		}

		process::flag::Leftrotatepressed = false;
		process::flag::Rightrotatepressed = false;

		/* 操作で移動される前の状態を保持 */
		process::minotoboard::bvertical = moving.vertical;
		process::minotoboard::bx = moving.x;
		process::minotoboard::by = moving.y;

		/* 一定時間経過で1マス下に落ちる */
		if (!process::flag::minowaiting && falling.sF() > fall_second) {
			/* ミノのどっちのブロックも地についていない場合のみ下に移動 */
			Logger << U"falling";
			before = moving;
			if (moving.y != 0 && block2y(static_cast<int>(moving.y), moving.vertical) != 0)
				moving.y--;
			else
				process::flag::minowaiting = true;
			falling.restart();
			sync();
		}
		/* 左入力キーが押されたとき */
		else if (input::Left.down()) {
			if (DEBUG) Logger << U"Left pressed";
			before = moving;
			if (moving.x != 0)
				moving.x--;
			sync();
		}
		/* 右入力キーが押されたとき */
		else if (input::Right.down()) {
			if (DEBUG) Logger << U"Right pressed";
			before = moving;
			if (moving.x != width::board::COL - 1 && !(!moving.vertical && moving.x == width::board::COL - 2))
				moving.x++;
			sync();
		}
		/* ホールドキーが押されたとき(ただしこの処理は各ターン一度のみ) */
		else if (input::Hold.down() && !process::flag::holdget) {
			/* ホールドに入れる前に初期化 */
			moving.x = 2;
			moving.y = width::board::ROW - 1;
			moving.vertical = false;
			if (process::flag::holdgenerated) {  /* 既にホールドがあった場合 */
				/* ホールドと操作中のミノを交換 */
				Mino temp = moving;
				moving = hold;
				hold = temp;
			}
			else {
				hold = moving;
				moving = next;
				next = Mino();
			}
			process::flag::holdgenerated = true;  /* ホールドが一度でも作られたらずっとtrue */
			process::flag::backUpdated = false;  /* 背景にホールドの表示 */
			process::flag::holdget = true;
			sync();
			falling.restart();
		}
		/* 右回転キーが押されたとき右に90°回転(ミノのブロック1を中心に回転させる) */
		else if (input::RotateR.down()) {
			before = moving;
			if (moving.vertical)  /* ミノが元から垂直方向の際 */
				std::swap(moving.block1, moving.block2);  /* ミノのブロック1とブロック2を交換 */
			moving.vertical = !moving.vertical;
			sync();
			process::flag::Rightrotatepressed = true;
		}
		/* 左回転キーが押されたとき左に90°回転(ミノのブロック1を中心に回転させる) */
		else if (input::RotateL.down()) {
			before = moving;
			if (!moving.vertical)  /* ミノが元から垂直方向の際 */
				std::swap(moving.block1, moving.block2);  /* ミノのブロック1とブロック2を交換 */
			moving.vertical = !moving.vertical;
			sync();
			process::flag::Leftrotatepressed = true;
		}
	}
	
	if (process::flag::minoground) falling.reset();
}

void MinoAdmin::sync() {
	/* 以下5個の処理はBoard用 */
	process::minotoboard::block1 = moving.block1;
	process::minotoboard::block2 = moving.block2;
	process::minotoboard::vertical = moving.vertical;
	process::minotoboard::x = moving.x;
	process::minotoboard::y = moving.y;
	/* 以下4個の処理はBackGround用 */
	process::minotoback::nextblock1 = next.block1;
	process::minotoback::nextblock2 = next.block2;
	process::minotoback::holdblock1 = hold.block1;
	process::minotoback::holdblock2 = hold.block2;
}
