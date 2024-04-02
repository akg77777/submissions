#include "stdafx.h"
#include "Back.h"

Back::RectColor::RectColor() :
	InnerC( Color{ 200, 200, 200 } ),
	OuterC(Palette::Gray)
	{ }

Back::Frame::Frame() :
	Lside(650),
	Wide(300),
	yNext(80),
	yDifficult(230),
	yHold(380),
	yScore(530),
	yCombo(830),
	yDown(980),
	RectNext({ Lside, yNext, Wide, yDifficult - yNext }),
	RectDifficult({ Lside, yDifficult, Wide, yHold - yDifficult }),
	RectHold({ Lside, yHold, Wide, yScore - yHold }),
	RectScore({ Lside, yScore, Wide, yCombo - yScore }),
	RectCombo({ Lside, yCombo, Wide, yDown - yCombo })
	{  }
