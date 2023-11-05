#pragma once

#include "info.h"

/* タイトル画面の管理を行うクラス */
class Title : public App::Scene {
	public:
		Title(const InitData& init);
		void update() override;
		void draw() const override;
	private:
		images::title title_image;  /* タイトル画像の保存先 */
};
