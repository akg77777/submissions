#pragma once

#include "info.h"

class Tutorial : public App::Scene {
	public:
		Tutorial(const InitData& init);
		void update() override;
		void draw() const override;
	private:
		images::tutorial tutorial_image;
};

