#include "Block.h"
/* Block構造体の関数の定義 */
Block::Block(Blockname b) : block(b), flush(false), twincle(false), mino(false) { }

Block Block::operator=(const Block& block) {
	Block ret;

	ret.block = block.block;
	ret.flush = false;
	ret.twincle = false;
	ret.mino = false;

	return ret;
}

Block Block::operator=(const Blockname& block) {
	Block ret;

	ret.block = block;
	ret.flush = false;
	ret.twincle = false;
	ret.mino = false;

	return ret;
}

Texture Block::address() {
	if (block == Blockname::Star)
		return block_image.Star;
	if (block == Blockname::Rick)
		return block_image.Rick;
	if (block == Blockname::Coo)
		return block_image.Coo;
	if (block == Blockname::Kine)
		return block_image.Kine;
	if (block == Blockname::Gooey)
		return block_image.Gooey;
	if (block == Blockname::Bomb)
		return block_image.Bomb;
	if (block == Blockname::twincle)
		return block_image.Twincle;
	if (block == Blockname::Hard)
		return block_image.Hard;
	return block_image.none;  /* ブロックがない場合 */
}
