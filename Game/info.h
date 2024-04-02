#pragma once

#include "Data.h"

constexpr bool DEBUG = true;  /* デバッグ用変数 */

/* 用いるシーンの一覧を保存 */
enum class State {
	Title,
	Tutorial,
	Game,
	Ending
};

/* 今回使うシーンマネージャークラス */
using App = SceneManager<State>;

/* ブロックの名前 */
const enum class Blockname {
	Star,	 /*  星ブロック  */
	Rick,	 /*    リック	 */
	Coo,	 /*     クー     */
	Kine,    /*    カイン    */
	Gooey,   /*    グーイ    */
	Bomb,	 /* 爆弾ブロック */
	Hard,	 /* 固いブロック */
	twincle, /*    きらきら  */
	none     /* ブロックなし */
};

namespace width {  /* サイズの管理を行う名前空間 */
	namespace window {
		constexpr uint h = 1000;  /* 縦 */
		constexpr uint v = 1000;  /* 横 */
	}
	namespace board {
		constexpr uint ROW = 9;  /* 縦に9マス */
		constexpr uint COL = 6;  /* 横に6マス */
		constexpr uint V = 900;  /* block::SIZE * ROW */
		constexpr uint H = 600;  /* block::SIZE * COL */
	}
	namespace outer {  /* 盤面の外枠の幅 */
		constexpr uint UD = 20; /* 下 */
		constexpr uint L = 10;  /* 左 */
	}
	namespace block {
		constexpr uint SIZE = 100;  /* ブロックの大きさ */
	}
}

/* 処理時間や実行するプロセスを管理するデータなどの含まれる名前空間 */
namespace process {
	namespace second {  /* 処理時間などを調整する名前空間 */
		extern double flushing;  /* 点滅時間 */
		extern double reflesh;  /* drop関数が用いられてから次の処理が行われるまでの時間 */
		extern double betwincle;  /* きらきらの表示時間 */
		extern double fallsecond;  /* ブロックが落下するまでにかかる時間 */
		extern double gameover;
		extern Stopwatch GameOver;  /* ゲームオーバー後の余韻の時間を測る */
	}
	namespace flag {  /* flag名前空間はMain.cppで初期化 */
		extern bool boardinit;  /* 盤面の初期化が完了していたらtrue */
		extern bool doarrange;
		extern bool doerase;
		extern bool doexplode;  /* arrangeフェーズの中で爆発させるフェーズならtrue */
		extern bool backUpdated;  /* スコアやコンボ数が更新されたかを保存 */
		extern bool flushing;  /* 盤面が今点滅しているかを保存 */
		extern bool maketwincle;  /* きらきらを生成するフェーズならtrue */
		extern bool twincleGenerated;  /* キラキラが生成された状態ならばtrue */
		extern bool boardrise;  /* このフラグが立ったら次のターンboard::riseBottom関数が呼ばれる */
		extern bool handsturn;  /* このフラグが立つと画面はマスターハンド?以外の操作を一旦停止 */
		extern bool callriseBottom;  /* マスターハンドの動きが終了したら立つ */
		extern bool minogenerated;  /* ミノを同時に複数生成されないようにするフラグ */
		extern bool minowaiting;  /* ミノが動こうとした場所に移動できない時true */
		extern bool calldrop;  /* ミノが地についてからBoard::isGrounded()が呼ばれるまでの間true */
		extern bool minocanmove;  /* 操作が行われた時にその操作を完了させることができる場所出るかを確認 */
		extern bool minoground;  /* ミノが設置する場所まで落ちていたらtrue */
		extern bool Fastpressed;  /* Fastキーが押されたらtrue */
		extern bool holdgenerated;  /* ホールドに保存されたらtrue */
		extern bool holdget;  /* 各ターンホールドの交換は一回だけにするためのフラグ */
		extern bool Leftrotatepressed;  /* 左回転キーが押されたらtrue次の動きの前には初期化 */
		extern bool Rightrotatepressed;  /* 右回転キーが押されたらtrue次の動きの前には初期化 */
		extern bool pausepressed;  /* ポーズボタンが押されたか */
		extern bool gameover;  /* arrangeフェーズが終わる際、または盤面が上昇する際に判定される */
	}
	namespace minotoboard {  /* MinoAdminからboardにデータを渡す際に用いる名前空間 */
		/* 現在落下中のミノの情報 */
		extern Blockname block1;
		extern Blockname block2;
		extern bool vertical;
		extern uint x;
		extern uint y;
		extern bool grounded;  /* 接地されるタイミングかを返す */
		extern bool bvertical;
		extern uint bx;  /* 操作前)の状態を */
		extern uint by;  /* 毎操作保存 */
	}
	namespace minotoback {  /* MinoAdminからBackGroundにデータを渡す際に用いる名前空間 */
		/* 現在待機中のミノ(next)の情報 */
		extern Blockname nextblock1;
		extern Blockname nextblock2;
		/* 現在ホールド中のミノ(hold)の情報 */
		extern Blockname holdblock1;
		extern Blockname holdblock2;
	}
}

/* 難易度に関係するデータ(Main関数内のsetting関数で管理) */
namespace difficulty {
	extern double sync_second;  /* 通常の落下速度 */
	extern uint start_height;  /* ゲーム開始時の盤面の最大の高さ */
}

/* 操作に対応するキーボード入力を管理する名前空間 */
namespace input {
	extern InputGroup Left;     /* ミノを左に移動 */
	extern InputGroup Right;    /* ミノを右に移動 */
	extern InputGroup Fast;     /* ミノを少し早めに落下させる */
	extern InputGroup Hold;     /* 操作中のミノをホールドのミノと交換 */
	extern InputGroup RotateR;  /* ミノを右方向に90°回転 */
	extern InputGroup RotateL;  /* ミノを左方向に90°回転 */
}

/* info.cppで定義した関数のプロトタイプ宣言 */
/* フラグや時間設定などの初期化(盤面の初期化後に呼び出す) */
extern void minoflag();
/* arrangeフェーズへの移行時に呼び出す */
extern void arrangeflag();
/* メルセンヌ・ツイスター関数 */
extern void initMT();
extern double getRand();
