#include "Ending.h"

Ending::Ending(const InitData& init) : IScene{ init },
									   FilePath{ U"example/ScoreRanking/data.csv" },
									   file(CSV{ U"example/ScoreRanking/data.csv" }),
									   opened(false)
{
	m_continueButton = Rect{ 50, 600, 400, 150 };
	m_continueTransition = Transition{ 0.3s, 0.3s };
	m_endButton = Rect{ 550, 600, 400, 150 };
	m_endTransition = Transition{ 0.3s, 0.3s };
}

void Ending::update() {
	/* ファイルの読み書き */
	if (!opened) {
		/* 今回のスコアの保存 */
		round_score = data::score.get();

		/* データの読み込み */
		for (size_t row = 0; row < file.rows(); row++) {
			for (size_t col = 0; col < file.columns(row); col++) {
				scoredata.push_back(Parse<uint>(file.get(row, col)));
			}
		}

		/* 今回のスコアも含む */
		scoredata.push_back(data::score.get());

		/* スコアの高い順にデータを取得 */
		std::sort(scoredata.begin(), scoredata.end(), std::greater());

		/* データをすべてCSVファイルに出力 */
		CSV temp;
		for (const auto& e : scoredata)
			temp.write(e);

		temp.save(FilePath);

		opened = true;
	}

	/* GUI(ボタン)の設定 */
	if (m_continueButton.leftClicked()) 
		changeScene(State::Game);
	if (m_endButton.leftClicked()) 
		System::Exit();
	if (m_continueButton.mouseOver() || m_endButton.mouseOver())
		Cursor::RequestStyle(CursorStyle::Hand);
}

void Ending::draw() const {
	/* 背景の表示 */
	Scene::SetBackground(ColorF{ Palette::Darkslateblue });
	ending_image.endingimage.draw(100, 760);

	/* スコアランキングの表示 */
	FontAsset(U"TitleFont")(U"今回のスコア").drawAt(Scene::CenterF().x, 30, Palette::Mistyrose);
	FontAsset(U"SubColumn")(round_score).drawAt(Scene::CenterF().x, 100, Palette::Mistyrose);
	FontAsset(U"TitleFont")(U"スコアランキング").drawAt(Scene::CenterF().x, Scene::CenterF().y - 300, Palette::Mistyrose);
	FontAsset(U"SubColumn")(U"1st.\n2nd.\n3rd.").draw(200, Scene::CenterF().y - 200, Palette::Mistyrose);
	if(scoredata.size() > 0)
		FontAsset(U"explain")(scoredata[0]).drawAt(Scene::CenterF().x, Scene::CenterF().y - 160, Palette::Mistyrose);
	if(scoredata.size() > 1)
		FontAsset(U"explain")(scoredata[1]).drawAt(Scene::CenterF().x, Scene::CenterF().y - 100, Palette::Mistyrose);
	if(scoredata.size() > 2)
		FontAsset(U"explain")(scoredata[2]).drawAt(Scene::CenterF().x, Scene::CenterF().y - 40, Palette::Mistyrose);

	m_continueButton.draw(ColorF{ Palette::Aqua }).drawFrame(3);
	FontAsset(U"Menu")(U"CONINUE").drawAt(m_continueButton.center());
	m_endButton.draw(ColorF{ Palette::Aqua }).drawFrame(3);
	FontAsset(U"Menu")(U"GAME END").drawAt(m_endButton.center());
}
