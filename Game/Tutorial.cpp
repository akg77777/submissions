#include "Tutorial.h"

Tutorial::Tutorial(const InitData& init) : IScene{ init } { }

void Tutorial::update() {
	const auto anykeys = Keyboard::GetAllInputs();

	/* 何らかのキーがキーボードから押されたら、または左クリックされたら */
	if (!anykeys.isEmpty() || MouseL.down()) {
		/* ゲーム画面に遷移 */
		changeScene(State::Game);
	}

}

void Tutorial::draw() const{
	tutorial_image.tutorialimage.resized(width::window::h, width::window::v).draw(0, 0);

	/* 以下画面に表示する文字列 */
	FontAsset(U"TitleFont")(U"ゲーム説明").drawAt(Scene::Center().x, 50, Palette::Darkgray);
	FontAsset(U"Explanation")(U"仲間ブロック").drawAt(200, 140, Palette::Darkgray);
	FontAsset(U"Explanation")(U"リック　クー　カイン  グーイ").draw(0, 160, Palette::Darkgray);

	String explain1{ U"同じ仲間ブロックで挟んで\n" };
	String explain2{ U"星ブロックを消そう!\n" };
	String explain3{ U"グーイは誰とでも組み合わせられるよ!\n" };
	String explain4{ U"連鎖をするときらきらが降ってくるよ\n" };
	String explain5{ U"大連鎖を狙って高得点を目指そう!\n" };
	String Explain = explain1 + explain2 + explain3 + explain4 + explain5;
	FontAsset(U"Explanation")(Explain).draw(5, 290, Palette::Palevioletred);

	FontAsset(U"explain")(U"ブロックの回転	: VまたはB").draw(650, 180, Palette::Darkgray);
	FontAsset(U"explain")(U"ホールドの利用	: Spaceキー").draw(650, 220, Palette::Darkgray);
	FontAsset(U"explain")(U"ブロックの移動	: 左右キー").draw(650, 260, Palette::Darkgray);
}
