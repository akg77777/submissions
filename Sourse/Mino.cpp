#include "Mino.h"

Mino::Mino() {
	initMT();  /* メルセンヌ・ツイスターの利用 */
	double seed1 = getRand();
	double seed2 = getRand();

	auto getRandblock = [](double seed) {
			uint elem = static_cast<uint>(seed * 1000);
			return static_cast<Blockname>(elem % 4);
		};

	block1 = getRandblock(seed1);
	block2 = getRandblock(seed2);
	vertical = false;
	x = 2;
	y = width::board::ROW - 1;
}

Mino::Mino(Blockname bn1, Blockname bn2) {
	block1 = bn1;
	block2 = bn2;
	vertical = false;
	x = 2;
	y = width::board::ROW - 1;
}

Mino Mino::operator=(const Mino& mino) {  /* Minoのコピーコンストラクタ */
	if (this != &mino) {
		block1 = mino.block1;
		block2 = mino.block2;
		vertical = mino.vertical;
		x = mino.x;
		y = mino.y;
	}

	return *this;
}
