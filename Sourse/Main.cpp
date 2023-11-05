/* このゲームのフェーズ遷移
 * これらの処理は全てinfo.hにあるprocess::flagにより実行順を管理
 * [ゲーム実行フェーズ]
 * [ミノの生成・操作フェーズ](ミノの管理は全てMinoクラスに一任)
 * ・新しいミノの生成
 * ・(ホールドのノードの入れ替え)
 * [盤面の操作フェーズ(arrangeフェーズ)]
 * ・消せるブロックがあれば削除 (erase)
 * ・ブロックをすべて地に付ける (drop)
 * ・コンボ数に応じてきらきらを生成 (comboProcess)
 * ・削除フェーズに移行(erase)
 * ・以上を繰り返し消せるブロックがなくなったらノードの落下フェーズに戻る
 * ・3ターンに1回盤面をせり上げさせる(BackGround::display()が管理)
 */
# include "stdafx.h"

/* info.hにて宣言されたextern指定子をつけた名前空間を初期化 */
namespace process {
	namespace second {  /* 処理時間などを調整する名前空間 */
		/* 盤面初期化前はすぐ変化させる */
		double flushing = 0;  /* 点滅時間 */
		double reflesh = 1.0;  /* drop関数が用いられてから次の処理が行われるまでの時間 */
		double betwincle = 0;  /* きらきらの表示時間 */
		double fallsecond = 0.6;  /* ミノが下に落ちるまでかかる時間 */
		double gameover = 0.5;  /* 以下のストップウォッチの時間がこの秒数経つとゲームが終了 */
		Stopwatch GameOver{ StartImmediately::No };
	}
	namespace flag {
		/* 最初は盤面初期化用のフラグの立て方をする */
		bool boardinit = false;
		bool doarrange = true;  /* 最初の盤面の初期化のために立てておく */
		bool doerase = true;
		bool doexplode = false;
		bool backUpdated = false;  /* スコアやコンボ数が更新されたかを保存 */
		bool flushing = false;
		bool maketwincle = false;
		bool twincleGenerated = false;
		bool boardrise = false;
		bool handsturn = false;
		bool callriseBottom = false;  /* trueになったらboard.arraange()が感知 */
		bool minogenerated = true;
		bool calldrop = false;
		bool minowaiting = false;
		bool minocanmove = true;
		bool minoground = false;
		bool Fastpressed = false;
		bool holdgenerated = false;  /* 一度でもtrueになればずっとtrueでOK */
		bool holdget = false;
		bool Leftrotatepressed = false;
		bool Rightrotatepressed = false;
		bool pausepressed = false;
		bool gameover = false;
	}
	namespace minotoboard {
		Blockname block1 = Blockname::none;
		Blockname block2 = Blockname::none;
		bool vertical = false;
		uint x = 255;
		uint y = 255;
		bool grounded = false;
		bool bvertical = false;
		uint bx = 255;
		uint by = 255;
	}
	namespace minotoback {
		Blockname nextblock1 = Blockname::none;
		Blockname nextblock2 = Blockname::none;
		Blockname holdblock1 = Blockname::none;
		Blockname holdblock2 = Blockname::none;
	}
}
namespace difficulty {  /* このデータはsetting関数にて変更可 */
	double sync_second = 0.8;  /* ブロックが落下してから実際にBoardオブジェクトに同期されるまでにかかる時間 */
	uint start_height;  /* 開始時の盤面の高さの最大値 */
}
namespace data {  /* それぞれコンストラクタにて初期化 */
	Data combo;
	Data score;
	Data turn;
}
namespace input {
	InputGroup Left = KeyLeft;
	InputGroup Right = KeyRight;
	InputGroup Fast = KeyDown;
	InputGroup Hold = KeySpace;
	InputGroup RotateR = KeyB;
	InputGroup RotateL = KeyV;
}

/* アプリ自体の設定を行う関数 */
void setting() {
	/* ユーザー操作のうちウィンドウの閉じる操作のみを終了操作とする */
	System::SetTerminationTriggers(UserAction::CloseButtonClicked);
	/* シーンサイズの設定 */
	Scene::Resize(width::window::v, width::window::h);
	/* デフォルトのウィンドウサイズ */
	Window::Resize(width::window::v, width::window::h);
	/* ウィンドウを手動でサイズ変更できるように */
	Window::SetStyle(WindowStyle::Sizable);
	/* ウィンドウのタイトル */
	Window::SetTitle(U"カービィのきらきらきっず");
	/* 表示するシーン外のレターボックスの色を指定 */
	Scene::SetLetterbox(ColorF{ Palette::Palevioletred });

	/* difficulty名前空間のデータ設定 */
	process::second::fallsecond = 1.0;  /* ブロックの接地してからの落下速度 */
	difficulty::start_height = 4;  /* ゲーム開始時の盤面の高さの最大値 */
}

/* Main関数 */
void Main() {
	setting();

	/* 用いるフォント */
	FontAsset::Register(U"TitleFont", FontMethod::MSDF, 60, Typeface::Black);
	FontAsset(U"TitleFont").setBufferThickness(4);
	FontAsset::Register(U"SubColumn", FontMethod::MSDF, 45, Typeface::Bold);
	FontAsset::Register(U"Explanation", FontMethod::MSDF, 30, Typeface::Medium);
	FontAsset::Register(U"explain", FontMethod::MSDF, 25, Typeface::Medium);
	FontAsset::Register(U"Menu", FontMethod::MSDF, 40, Typeface::Black);

	App manager;
	manager.add<Title>(State::Title);
	manager.add<Tutorial>(State::Tutorial);
	manager.add<Game>(State::Game);
	manager.add<Ending>(State::Ending);

	/* タイトル画面から開始 */
	manager.init(State::Title);

	while (System::Update()) {
		if (not manager.update()) {
			break;
		}
	}
}
