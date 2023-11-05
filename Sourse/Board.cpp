#include "Board.h"

/* Board構造体メンバ関数の定義 */
Board::Board() {
	using namespace width::board;

	/* 動的メモリの確保 */
	map = new Block * [COL];
	for (uint i : step(COL))
		map[i] = new Block[ROW];

	updated = false;
	explodeflag = false;
	exploded = 0;
	boardfilled = false;
	flagreset();
	flush_time.reset();
	reflesh_time.reset();
}

void Board::flagreset() {
	flushflag = false;
	explodeflag = false;
	process::flag::twincleGenerated = false;

	for (uint i : step(width::board::COL)) {
		for (uint j : step(width::board::ROW)) {
			map[i][j].flush = false;
			map[i][j].twincle = false;
			map[i][j].mino = false;
			map[i][j].bomb = false;
		}
	}
}

Board::~Board() {
	using width::board::ROW, width::board::COL;
	for (uint i : step(COL)) {
		delete[] map[i];
		map[i] = nullptr;
	}
	delete[] map;
}

void Board::init() {
	if (!boardfilled) {
		fillRandBoard();
		boardfilled = true;
	}
	arrange();

	if (!process::flag::doarrange && !inSafe()) {  /* 初期化してもゲームオーバーになるような盤面であった場合 */
		boardfilled = false;
		process::flag::boardinit = false;
		process::flag::doarrange = true;
		process::flag::doerase = true;
		process::flag::backUpdated = false;
		process::flag::maketwincle = false;
		process::flag::twincleGenerated = false;
		flagreset();
	}
	if (!process::flag::doarrange) {  /* 初期化処理が終わったら */
		minoflag();
		flagreset();
		updated = false;
		data::score = 0;
		data::turn = 1;
	}
}

/* ゲーム開始後の盤面の操作は全てこれで管理(arrngeフェーズ) */
void Board::arrange() {
	/* ミノフェーズ中の処理 */
	if (!process::flag::doarrange && process::flag::minogenerated) {/* ミノの落下フェーズ中も接地しないか判定は行う */
		if (DEBUG) Logger << U"Mino";
		if (!process::flag::minoground) {
			judgeMino();  /* その場所に移動できるか判定 */
			PopMino();  /* ミノを複数表示しないため */
			MinoInsert();  /* 場所の変わったミノの表示 */
			if (process::flag::calldrop && !reflesh_time.isStarted())
				reflesh_time.restart();
			if (reflesh_time.sF() > difficulty::sync_second) {
				isGrounded();  /* ミノが地に着いた時のための関数 */
				reflesh_time.reset();
			}
		}
		if(process::flag::minoground) {
			drop();
			if (reflesh_time.sF() > process::second::reflesh) {
				if (DEBUG) Logger << U"endingg";
				flagreset();  /* arrangeフェーズに向けフラグのリセット */
				arrangeflag();  /* ミノが接地されたらarrangeフェーズに移行 */
			}
		}
	}
	/* 地面についたらprocess::flag::doarrangeフラグが立つのでそのまま直後のarrangeフェーズが始まる */

	/* arrangeフェーズ中の盤面の整理を行う処理 */
	if (process::flag::doarrange) {  /* arrangeフラグの立っているときのみ */
		if (!process::flag::maketwincle) {  /* ブロックの削除フェーズ */
			if (process::flag::doerase) {
				erase();
				/* 点滅終了まで待つ */
				if (flush_time.sF() > process::second::flushing) {
					process::flag::flushing = false;
					flushflag = false;
					flush_time.reset();
					reflesh_time.reset();
					process::flag::doerase = false;  /* drop関数に移行 */
					if (!explodeflag) {  /* 爆弾ブロックが削除されていなければ */
						flagreset();
					}
					else {
						process::flag::doexplode = true;
					}
				}
			}
			if (process::flag::doexplode) {  /* 爆発の処理 */
				exploded++;
				explosion();
				if (exploded == width::board::COL) {
					exploded = 0;
					process::flag::doexplode = false;
					flagreset();
					reflesh_time.reset();
				}
			}
			else if (!process::flag::doerase) {  /* doerase == falseの際drop関数実行 */
				drop();  /* board.reflesh_time.start()も行う */
				if (reflesh_time.sF() > process::second::reflesh) {
					process::flag::doerase = true;  /* erase関数に移行 */
				}
			}
		}
		if (process::flag::maketwincle) {  /* きらきらの生成フェーズ */
			comboProcess();
		}
	}
}

void Board::draw() const {
	if (process::flag::pausepressed)
		return;

	if (!process::flag::boardinit) return;   /* 初期化されるまで表示しない */

	using namespace width;
	static Texture current[board::COL][board::ROW];
	double T = Periodic::Sawtooth1_1(0.1s);  /* 点滅周期などに用いる */

	/* 盤面が更新されたら、点滅していない時のみ画像読み込み */
	if (!updated && !flushflag) {
		for (uint i : step(board::COL)) {
			for (uint j : step(board::ROW)) {
				if (map[i][j].block != Blockname::none) {
					/* ブロックがあれば読み込む */
					current[i][j] = map[i][j].address();
					updated = true;
				}
			}
		}
	}

	if (flushflag) {  /* 点滅させる場合 */
		if (DEBUG) Logger << U"flush";
		for (uint i : step(board::COL)) {  /* 横 */
			for (uint j : step(board::ROW)) {  /* 縦 */
				/* 表示位置をx, yに保存 */
				uint x = outer::L + (i * block::SIZE);
				uint y = window::h - ((j + 1) * block::SIZE + outer::UD);
				if (map[i][j].block == Blockname::none && !map[i][j].flush)
					continue;  /* 元からなかったブロックはスキップ */
				if (map[i][j].flush && T > 0.0) {
					current[i][j].resized(block::SIZE, block::SIZE)
						.draw(x, y, Alpha(10.0));
				}
				else if (map[i][j].flush && T < 0.0)
					current[i][j].resized(block::SIZE, block::SIZE).draw(x, y);
				else
					current[i][j].resized(block::SIZE, block::SIZE).draw(x, y);
			}
		}
	}
	else {  /* 通常の表示 */
		for (uint i : step(board::COL)) {  /* 横 */
			for (uint j : step(board::ROW)) {  /* 縦 */
				uint x = outer::L + (i * block::SIZE);
				uint y = window::h - ((j + 1) * block::SIZE + outer::UD);
				if (map[i][j].block == Blockname::twincle && T > 0.0)  /* きらきらは点滅、回転させながら表示させる */
					current[i][j].resized(block::SIZE * (2 / 3), block::SIZE * (2 / 3)).rotated(Scene::Time() * 720_deg).draw(x, y);
				else if (map[i][j].block != Blockname::none)
					current[i][j].resized(block::SIZE, block::SIZE).draw(x, y);
			}
		}
	}
}

void Board::fillRandBoard() {
	if (DEBUG) Logger << U"fill";

	for (uint i : step(width::board::COL)) {
		for (uint j : step(difficulty::start_height)) {
			Blockname newBlock = genRandBlock();
			insert(newBlock, i);
		}
	}

	updated = false;
}

bool Board::inSafe() {
	/* 盤面上の最上列の真ん中2マスのどちらかにブロックがあったらゲームオーバー */
	bool ret
		= ((map[2][8].block == Blockname::none || map[2][8].block == Blockname::twincle || map[2][8].mino)
			&& (map[3][8].block == Blockname::none || map[3][8].block == Blockname::twincle || map[3][8].mino));

	return ret;
}

/* private: */
void Board::judgeMino() {
	process::flag::minocanmove = true;
	process::flag::calldrop = false;

	auto insafe = [](uint x, uint y) {
			return (x < width::board::COL && y < width::board::ROW);
		};

	/* ミノのそれぞれのブロックの座標の獲得 */
	uint block1x = process::minotoboard::x, block1y = process::minotoboard::y;
	uint block2x, block2y;  /* ミノが垂直かどうかでブロック2の場所を調べる */
	if (process::minotoboard::vertical) {
		block2x = block1x;
		block2y = block1y - 1;
	}
	else {
		block2x = block1x + 1;
		block2y = block1y;
	}

	/* 移動しようとしている場所にミノを動かせない時を調べる */
	if ((!insafe(block1x, block1y) || !insafe(block2x, block2y))
		|| (map[block1x][block1y].block != Blockname::none && !map[block1x][block1y].mino)
		|| (map[block2x][block2y].block != Blockname::none && !map[block2x][block2y].mino)) {
		process::flag::minocanmove = false;
	}

	/* 移動させることが無理な場合はその前の座標データを用いる */
	if (!process::flag::minocanmove) {
		block1x = process::minotoboard::bx, block1y = process::minotoboard::by;
		if (process::minotoboard::bvertical) {
			block2x = block1x;
			block2y = block1y - 1;
		}
		else {
			block2x = block1x + 1;
			block2y = block1y;
		}
	}

	/* ミノをそこに移動できた際にミノを下に落とすdrop関数を呼ぶべきか調べる */
	if ((!insafe(block1x, block1y - 1) || !insafe(block2x, block2y - 1))
		|| (map[block1x][block1y - 1].block != Blockname::none && !map[block1x][block1y - 1].mino)
		|| (map[block2x][block2y - 1].block != Blockname::none && !map[block2x][block2y - 1].mino)) {
		Logger << U"calldrop";
		process::flag::calldrop = true;
	}

	if (!process::flag::calldrop) {
		if (DEBUG) Logger << U"call reset";
		reflesh_time.reset();
	}
}

void Board::PopMino() {
	/* ミノが動かせない時 */
	if (!process::flag::minocanmove) return;

	if (DEBUG) Logger << U"popmino";

	/* ミノが動かせるとき前回表示したミノを削除 */
	uint block1x = process::minotoboard::bx, block1y = process::minotoboard::by;
	uint block2x, block2y;
	if (process::minotoboard::bvertical) {
		block2x = block1x;
		block2y = block1y - 1;
	}
	else {
		block2x = block1x + 1;
		block2y = block1y;
	}
	map[block1x][block1y].block = Blockname::none;
	map[block2x][block2y].block = Blockname::none;
	map[block2x][block2y].mino = false;
	map[block2x][block2y].mino = false;

	updated = false;
}

void Board::MinoInsert() {
	if (!process::flag::minocanmove) return;

	if (DEBUG) Logger << U"minoinsert";
	auto insafe = [](uint x, uint y) {
			return (x < width::board::COL && y < width::board::ROW);
		};

	int block1x = process::minotoboard::x, block1y = process::minotoboard::y;
	int block2x, block2y;

	if (!process::minotoboard::vertical)  /* ミノが水平方向になっている場合 */
		block2x = block1x + 1, block2y = block1y;
	else
		block2x = block1x, block2y = block1y - 1;

	if (!insafe(block1x, block1y) || !insafe(block2x, block2y)) {
		throw Error{ U"ミノが盤面の範囲外にあります" };
		exit(1);
	}

	/* ブロックの設置 */
	map[block1x][block1y].block = process::minotoboard::block1;
	map[block1x][block1y].mino = true;
	map[block2x][block2y].block = process::minotoboard::block2;
	map[block2x][block2y].mino = true;

	updated = false;
}

void Board::isGrounded() {
	using namespace process::minotoboard;  /* minotoboard名前空間を呼び、minoのデータをそのまま使っているようにする */

	if (DEBUG) Logger << U"isGrounded?";
	bool isgrounded = false;
	uint top = 255;  /* そもそも盤面に空きスペースがない場合のことも考慮して調べる */

	for (uint x : step(width::board::COL)) {
		if (map[x][0].mino) { /* 底にミノが着いた場合 */
			isgrounded = true;
		}
	}

	for (uint x : step(width::board::COL)) {
		for (uint y = 1; y < width::board::ROW; y++) {
			if (map[x][y].mino && map[x][y - 1].block != Blockname::none) {
				isgrounded = true;
			}
		}
	}

	/* フラグのリセット */
	process::flag::minowaiting = false;
	process::flag::calldrop = false;
	flagreset();

	if (isgrounded)
		process::flag::minoground = true;
}

void Board::erase() {
	/*
	 * 今回このゲームではブロックの消える条件を同じ仲間ブロックの間だけ消える訳でなく
	 * グーイは他のどの仲間ブロックともつなげて消せることができるようにしている
	 */
	using namespace width::board;
	if (DEBUG) Logger << U"erase";
	uint getscore = 0;  /* 今回のerase処理で得たスコアを保存 */

	bool erased = false;  /* ブロックが実際消されたか */

	/* UnionFind木の利用 */
	UnionFind uf(COL * ROW);

	/* 今回使う関数群 */
	/* ((x, y) -> y * COL + x) */
	auto topoint = [](uint x, uint y) { return static_cast<uint>(y * COL + x); };

	/* topoint関数の逆 */
	auto revpoint = [](uint z) { return std::make_pair<uint>(z % COL, z / COL); };

	/* 引数の座標が盤面の範囲内か調べる関数 */
	auto insafe = [](uint x, uint y) {
			return (x >= 0 && x < COL) && (y >= 0 && y < ROW);
		};

	constexpr int dx[4] = { 1, 0, -1, 0 };
	constexpr int dy[4] = { 0, 1, 0, -1 };

	/* 仲間ブロック(リック・クー・カイン・グーイ)の場所をすべて保存 */
	std::queue<uint> Q;

	bool null[COL][ROW] = { false };  /* ブロックのない場所の保存 */
	bool partner[COL][ROW] = { false };  /* そのブロックが仲間ブロックか保存 */

	/* 仲間ブロックの探索 */
	for (uint i : step(COL)) {
		for (uint j : step(ROW)) {
			if (map[i][j].block == Blockname::Rick || map[i][j].block == Blockname::Coo
				|| map[i][j].block == Blockname::Kine || map[i][j].block == Blockname::Gooey) {
				partner[i][j] = true;
				Q.push(topoint(i, j));
			}
			/* ブロックのない場所の保存 */
			if (map[i][j].block == Blockname::none)
				null[i][j] = true;
		}
	}

	/* 全ての仲間ブロックを調べ消せるまとまりがあるか探索 */
	while (!Q.empty()) {
		uint p = Q.front();  /* 先頭の要素を調べる */
		Q.pop();
		if (uf.rank[p] != 1) continue;  /* 既にUnion-Findに登録されていたらスキップ */
		auto [cx, cy] = revpoint(p);
		Blockname cblock = map[cx][cy].block;  /* 移動前のブロックの保存 */

		for (uint i : step(4)) {  /* 調べる方向の決定 */
			for (int ex = cx + dx[i], ey = cy + dy[i];
				insafe(ex, ey) && !null[ex][ey]; ex += dx[i], ey += dy[i]) {
				/* 仲間ブロック見つけるまでスキップ */
				if (!partner[ex][ey]) continue;
				/* グーイまたは同じ仲間ブロックを見つけたらそこは消去対象 */
				if (map[ex][ey].block != cblock && map[ex][ey].block != Blockname::Gooey) break;

				erased = true;

				/* 同じ仲間ブロックを見つけたらUnion-Findに保存 */
				for (int x = cx, y = cy; insafe(x, y) && (x != ex + dx[i] || y != ey + dy[i]); x += dx[i], y += dy[i]) {
					uf.unite(p, topoint(x, y));
					if (map[x][y].block == Blockname::Bomb) {  /* 爆弾ブロックが消せたら */
						map[x][y].bomb = true;  /* 消される爆弾ブロックの場所を保存 */
						explodeflag = true;
					}
				}
			}
		}
	}

	popTwincle();  /* きらきらがあれば削除 */

	if (!erased) {  /* ブロックの削除フェーズが終了したら */
		if (!flushflag) { /* この段階で消せるブロックがないとき、きらきらフェーズに遷移 */
			if (DEBUG) Logger << U"noflush";
			if (process::flag::twincleGenerated) {  /* きらきらを生成したうえで消せるブロックがない場合 */
				minoflag();
				data::combo.reset();
				data::turn++;
				reflesh_time.reset();
				if (!inSafe()) {
					if (DEBUG) Logger << U"erased gameover";
					process::flag::gameover = true;
				}
				if (process::flag::gameover && inSafe())
					process::flag::gameover = false;
				return;
			}
			process::flag::maketwincle = true;
			reflesh_time.reset();
		}
		return;
	}

	flushflag = true;  /* 点滅フラグを立てる */

	/* 以降ブロックの削除と点滅 */
	Array<uint> idx[COL * ROW];  /* 各グループを根を基準にまとめる */
	for (uint i : step(COL* ROW)) idx[uf.root(i)].push_back(i);

	for (uint i : step(COL* ROW)) {  /* 点滅させるブロックの保存 */
		if (idx[i].size() < 2) continue;
		for (uint& j : idx[i]) {
			auto [x, y] = revpoint(j);
			map[x][y].flush = true;
			if (map[x][y].block == Blockname::Star)
				getscore++;
		}
	}

	/* ブロック削除の処理 */
	for (uint i : step(COL* ROW)) {
		if (idx[i].size() < 2) continue;  /* 根でない、または1ブロックのみのグループの場合スキップ */
		for (uint& j : idx[i]) {
			auto [x, y] = revpoint(j);  /* 座標の読み込み */
			/* 固いブロックの処理 */
			if (map[x][y].block == Blockname::Hard) {
				map[x][y].block = Blockname::Star;
				continue;
			}
			map[x][y].block = Blockname::none;
		}
	}

	if (process::flag::boardinit) {
		data::score = getscore + data::score.get();
		data::combo++;  /* 消せたらコンボ数を加算 */
	}

	process::flag::backUpdated = false;

	process::flag::flushing = true;
	updated = false;  /* 盤面が更新されたか */
	flush_time.restart();
}

void Board::explosion() {
	if (DEBUG) Logger << U"explode";

	uint getscore = 0;

	auto insafe = [](uint x, uint y) {
			return (x < width::board::COL && y < width::board::ROW);
		};

	for (uint x : step(width::board::COL)) {
		for (uint y : step(width::board::ROW)) {
			if (map[x][y].bomb) {
				Logger << U"(x,y) = " << x << U" " << y;
				if (insafe(x - exploded, y)) {
					if (map[x - exploded][y].block == Blockname::Star)
						getscore++;
					map[x - exploded][y].block = Blockname::none;
				}
				if (insafe(x + exploded, y)) {
					if (map[x + exploded][y].block == Blockname::Star)
						getscore++;
					map[x + exploded][y].block = Blockname::none;
				}
			}
		}
	}

	data::score = data::score + getscore;

	process::flag::backUpdated = false;

	updated = false;

	if (!reflesh_time.isStarted())
		reflesh_time.restart();
}

void Board::drop() {
	bool dropped = false;  /* ブロックを実際落としたか */
	if (DEBUG) Logger << U"drop";

	auto getSpace = [&](int x, int y) {  /* 座標(x, y)以下にある空きスペースの場所を返す */
			uint ret = 255;
			for (uint i : step(y)) {
				if (map[x][i].block == Blockname::none)
					ret = i;
			}
			return ret;
		};

	/* この条件式内の処理はomboProcess関数の処理内のみで実行される */
	if (process::flag::twincleGenerated) {  /* きらきらの処理を行う場合 */
		for (uint i : step(width::board::COL)) {
			for (uint j : step(width::board::ROW)) {
				if (map[i][j].block != Blockname::twincle || getSpace(i, j) == 255) continue;
				map[i][getSpace(i, j)].block = Blockname::twincle;
				map[i][j].block = Blockname::none;
				dropped = true;
			}
		}
		if (dropped) {
			updated = false;
			reflesh_time.restart();  /* ブロックを落としたときのみreflesh_timeの再開 */
		}
		return;
	}

	for (uint i : step(width::board::COL)) {
		for (uint j : step(width::board::ROW)) {
			if (map[i][j].block == Blockname::none || getSpace(i, j) == 255) continue;
			/* 下に空きスペースがあった場合 */
			map[i][getSpace(i, j)].block = map[i][j].block;
			map[i][j].block = Blockname::none;
			dropped = true;
		}
	}

	if (!reflesh_time.isStarted())
		reflesh_time.restart();

	if (dropped) {
		if (DEBUG) Logger << U"drop success";
		updated = false;  /* 盤面が更新 */
		return;
	}

	/* ブロックが落ちなかった場合、つまりerase関数でブロックが消されたが落とせるブロックがなかった際 */
	process::flag::doerase = true;
}

uint Board::getTop(uint x) {  /* x座標で下から見て最初にブロックのない座標を返す */
	uint idx = static_cast<uint>(255);

	for (uint i : step(width::board::ROW)) {
		if (map[x][i].block == (Blockname::none)) {
			idx = i;
			break;
		}
	}

	return idx;
}

void Board::insert(Blockname newblock, uint x) {
	if(getTop(x) != 255)
		map[x][getTop(x)].block = newblock;
}

Blockname Board::genRandBlock() {
	initMT();  /* メルセンヌ・ツイスターの利用 */
	double seed = getRand();

	if (seed >= 60) return Blockname::Star;
	if (seed >= 50) return Blockname::Gooey;
	if (seed >= 35) return Blockname::Rick;
	if (seed >= 20) return Blockname::Coo;
	if (seed >= 5) return Blockname::Kine;
	return Blockname::Bomb;

	/* [生成率の設定]
	 * 星ブロック	:40%
	 * グーイ		:10%
	 * リック		:15%
	 * クー			:15%
	 * カイン		:15%
	 * 爆弾ブロック	: 5%
	 */
}

/* 生成しないブロックを指定する際のみ呼び出される */
Blockname Board::genRandBlock(Array<Blockname> ar) {
	initMT();  /* メルセンヌ・ツイスターの利用 */
	double seed = getRand();  /* 0-100の範囲の実数をランダムに設定 */

	uint num = ar.size();  /* 生成できるブロックの数 */

	switch (num) {
		case 4:  /* 生成できないブロックがグーイともう一つの仲間ブロックだけの際 */
			if (seed >= 70) return ar[0];  /* 星ブロック */
			if (seed >= 40) return ar[1];
			if (seed >= 10) return ar[2];
			else return ar[3];  /* 爆弾ブロック */
		case 3:  /* 生成できる仲間ブロックが一つしかない場合 */
			if (seed >= 65) return ar[0];  /* 星ブロック */
			if (seed >= 20) return ar[1];
			else return ar[2];  /* 爆弾ブロック */
		case 2:
			if (seed >= 30) return ar[0];
			else return ar[1];
	}
}

void Board::riseBottom() {
	if (DEBUG) Logger << U"riseBottom";

	initMT();  /* ランダム生成にメルセンヌ・ツイスターの利用 */
	Blockname genBottom[width::board::COL] = { Blockname::none };  /* 底に生成するブロックを一旦保存 */

	auto insafe = [](uint x, uint y) {  /* 指定した座標がしっかり盤面の範囲内に入っているかを調べる関数 */
			return ((x >= 0 && x < width::board::COL) && (y >= 0 && y < width::board::ROW));
		};

	auto getblock = [&](uint x) {  /* 引数に対して上方向・左右を見てそこにブロックが消されずに置けるブロック群を返す */
		/* その場所に配置可能なブロックの候補を保存(ArrayはSiv3Dにおけるvector型) */
		Array<Blockname> temp = { Blockname::Star, Blockname::Rick, Blockname::Coo,
			Blockname::Kine, Blockname::Gooey, Blockname::Bomb };

		Array<Blockname> erasing;  /* tempから削除する要素 */

		for (uint y : step(getTop(x))) {  /* 上方向に探索 */
			Blockname b = map[x][y].block;
			if (b == Blockname::Rick || b == Blockname::Coo || b == Blockname::Kine) {
				erasing.push_back(b);
				erasing.push_back(Blockname::Gooey);
				break;
			}
			if (b == Blockname::Gooey) {
				for (uint i = 1; i < 5; i++) {  /* 仲間ブロックはどれも挿入不可能 */
					Blockname elem = static_cast<Blockname>(i);
					erasing.push_back(elem);
				}
				erasing.erase(std::unique(erasing.begin(), erasing.end()), erasing.end());  /* 重複する要素の削除 */
				for (auto& e : erasing) {
					auto newend = remove(temp.begin(), temp.end(), e);
					temp.erase(newend, temp.end());
				}
				return temp;
			}
		}
		for (int i = static_cast<int>(x); i >= 0; i--) {  /* 左方向を探索 */
			if (i == x) continue;

			Blockname b = genBottom[i];
			if (b == Blockname::Rick || b == Blockname::Coo || b == Blockname::Kine) {
				erasing.push_back(b);
				erasing.push_back(Blockname::Gooey);
				break;
			}
			if (b == Blockname::Gooey) {
				for (uint i = 1; i < 5; i++) {  /* 仲間ブロックはどれも挿入不可能 */
					Blockname elem = static_cast<Blockname>(i);
					erasing.push_back(elem);
				}
				erasing.erase(std::unique(erasing.begin(), erasing.end()), erasing.end());  /* 重複する要素の削除 */
				for (auto& e : erasing) {
					auto newend = remove(temp.begin(), temp.end(), e);
					temp.erase(newend, temp.end());
				}
				return temp;
			}
		}
		for (uint i = (x); i < width::board::COL; i++) {  /* 右方向を探索 */
			if (i == x) continue;
			Blockname b = genBottom[i];
			if (b == Blockname::Rick || b == Blockname::Coo || b == Blockname::Kine) {
				erasing.push_back(b);
				erasing.push_back(Blockname::Gooey);
				break;
			}
			if (b == Blockname::Gooey) {
				for (uint i = 1; i < 5; i++) {  /* 仲間ブロックはどれも挿入不可能 */
					Blockname elem = static_cast<Blockname>(i);
					erasing.push_back(elem);
				}
				erasing.erase(std::unique(erasing.begin(), erasing.end()), erasing.end());  /* 重複する要素の削除 */
				for (auto& e : erasing) {
					auto newend = remove(temp.begin(), temp.end(), e);
					temp.erase(newend, temp.end());
				}
				return temp;
			}
		}
		erasing.erase(std::unique(erasing.begin(), erasing.end()), erasing.end());  /* 重複する要素の削除 */
		for (auto& e : erasing) {
			auto newend = remove(temp.begin(), temp.end(), e);
			temp.erase(newend, temp.end());
		}
		return temp;
	};

	/* 生成する場所に偏りをなくすために乱数を生成してブロックの挿入場所からランダムに決める */
	uint from = static_cast<uint>(getRand()) % width::board::COL;

	for (uint i : step(width::board::COL)) {
		if (getblock((from + i) % width::board::COL).size() == 6) {  /* 特に生成するブロックに指定がない場合 */
			genBottom[(from + i) % width::board::COL] = genRandBlock();
			continue;
		}
		genBottom[(from + i) % width::board::COL] = genRandBlock(getblock((from + i) % width::board::COL));
	}
	/* 実際に行うブロックの挿入 */
	for (uint i : step(width::board::COL)) {
		for (int j = static_cast<int>(getTop(i)); j >= 0; j--) {
			if (!insafe(i, j + 1) || !insafe(i, j)) continue;
			map[i][j + 1].block = map[i][j].block;
			if (j == 0) map[i][j].block = genBottom[i];
		}
	}

	if (!inSafe())
		process::flag::gameover = true;
	if (process::flag::gameover && inSafe())
		process::flag::gameover = false;

	updated = false;  /* 盤面は更新済み */
	arrangeflag();
	process::flag::callriseBottom = false;
	process::flag::handsturn = false;
}

/* 以下きらきらの処理を行う関数群 */
void Board::comboProcess() {
	if (DEBUG) Logger << U"comboprocess";
	if (data::combo.get() < 2) {  /* コンボ数が2未満の場合arrangeフェーズは終了 */
		minoflag();  /* minoフェーズに移行 */
		data::combo.reset();
		data::turn++;
		reflesh_time.reset();
		if (!inSafe()) {
			if (DEBUG) Logger << U"combo gameover";
			process::flag::gameover = true;
		}
		if (process::flag::gameover && inSafe())
			process::flag::gameover = false;
		return;
	}

	if (process::flag::maketwincle) {  /* フラグの立っている時のみ */
		if (!process::flag::twincleGenerated)  /* きらきらが今回まだ生成されていなかった場合 */
			genTwincle();  /* きらきらを生成 */
		drop();  /* きらきらを地に付ける */
		if (reflesh_time.sF() > process::second::betwincle) {  /* 一定時間たったら */
			toStar();  /* 地に着いたきらきらを星ブロックに */
			process::flag::maketwincle = false;  /* きらきらの生成フェーズの終了 */
		}
	}
	else  /* フラグが立っていないのに実行しようとしていたらエラーを表示させる */
		throw Error{ U"mustn't call comboprocess" };
}

void Board::genTwincle() {
	initMT();  /* メルセンヌ・ツイスターの利用 */
	if (DEBUG) Logger << U"genTwincle";
	uint num = data::combo.getTwincle();  /* 生成するきらきらの数 */

	if(process::flag::boardinit)
		data::score = data::score + num;  /* きらきら分スコアを加算 */
	process::flag::backUpdated = false;

	process::flag::twincleGenerated = true;

	if (num == 6 || num == 12) {  /* 5コンボ以上の際の処理 */
		for (uint i : (step(num))) {
			if (getTop(i % 6) == 255)  /* 置ける場所がない場合 */
				continue;
			/* 一旦場面の最上にきらきらを生成 */
			map[i % 6][width::board::ROW - (i / 6 + 1)].block = Blockname::twincle;
		}
		updated = false;
		return;
	}

	/* 2コンボ以上4コンボ未満の場合はキラキラが個数分、底の低いところから順に生成される */
	Array<uint> order(6);  /* 低い場所から順に保存 */
	Array<uint> randAr(6);  /* そこの高さが同じ場合にこの乱数を入れた配列を用いて順序を決める */
	for (auto& e : randAr) e = getRand();
	std::iota(order.begin(), order.end(), 0);
	std::sort(order.begin(), order.end(), [&](uint lhs, uint rhs) {
			if (getTop(lhs) == getTop(rhs)) return(randAr[lhs] < randAr[rhs]);
			return (getTop(lhs) < getTop(rhs));
		});

	for (uint i : step(num)) { /* 底の低いほうから順に生成 */
		if (getTop(order[i]) == 255) continue;  /* 置けない場合 */
		map[order[i]][width::board::ROW - 1].block = Blockname::twincle;
	}

	updated = false;
}

void Board::toStar() {
	for (uint i : step(width::board::COL)) {
		for (uint j : step(width::board::ROW)) {
			if (map[i][j].block == Blockname::twincle) { /* きらきらの場所を */
				map[i][j].block = Blockname::Star;  /* 星ブロックに変換 */
				map[i][j].twincle = true;  /* フラグも立てる(popTwincle関数で削除するきらきらを探すため) */
			}
		}
	}
	updated = false;
}

void Board::popTwincle() {  /* erase関数内で呼び出される */
	if(DEBUG) Logger << U"POP?";
	if (!process::flag::twincleGenerated) return;
	if(DEBUG)  Logger << U"POP!";

	for (uint i : step(width::board::COL)) {
		for (uint j : step(width::board::ROW)) {
			if (map[i][j].twincle)
				map[i][j].block = Blockname::none;
		}
	}

	updated = false;  /* 必ずきらきらが一つ以上削除されるので盤面の更新 */
}
