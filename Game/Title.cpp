#include "Title.h"

Title::Title(const InitData& init) : IScene { init } { }

void Title::update() {
	const auto anykeys = Keyboard::GetAllInputs();

	/* 何らかのキーがキーボードから押されたら、または左クリックされたら */
	if (!anykeys.isEmpty() || MouseL.down()) {
		/* チュートリアル画面に遷移 */
		changeScene(State::Tutorial);
	}
}

/* タイトル画面の表示 */
void Title::draw() const {
	/* 背景画像の描画 */
	title_image.Titleimage.resized(width::window::v, width::window::h).draw(0, 0);

	FontAsset(U"TitleFont")(U"PUSH START").drawAt(Scene::CenterF(), ColorF{ 0.25 });
}
