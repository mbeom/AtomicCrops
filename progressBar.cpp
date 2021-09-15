#include "stdafx.h"
#include "progressBar.h"

HRESULT progressBar::init(float x, float y, int width, int height)
{
    _x = x;
    _y = y;

    _rcProgress = D2DMANAGER->RectMakeCenter(x, y, width, height);

    _progressBarTop = IMAGEMANAGER->addImage("frontBar", L"image/UI/bossHealthBar.png", width, height);
    _progressBarBottom = IMAGEMANAGER->addImage("backBar", L"image/UI/bossHealthFrame.png", width, height);

    _width = _progressBarTop->getWidth();
	_barName = L"괴물깍지";

    return S_OK;
}

void progressBar::release()
{
}

void progressBar::update()
{
    _rcProgress = D2DMANAGER->RectMakeCenter(_x, _y + _progressBarTop->getHeight() / 2, _progressBarTop->getWidth(), _progressBarTop->getHeight());
}

void progressBar::render()
{
    IMAGEMANAGER->findImage("backBar")->render(
        _rcProgress.left,
        _y + _progressBarTop->getHeight() / 2, 0, 0,
        _progressBarBottom->getWidth(), _progressBarBottom->getHeight());

    //얘는 게이지 크기가 조절되어야하기때문에 가로크기(sourWidth)에 _width 변수 써줌
	IMAGEMANAGER->findImage("frontBar")->render(
        _rcProgress.left,
        _y + _progressBarTop->getHeight() / 2, 0, 0,
        _width, _progressBarBottom->getHeight());

	D2DMANAGER->DrawTextD2DCenter(D2DMANAGER->white_brush, L"메이플스토리", 14, _barName.c_str(), 
		_rcProgress.left,
		(_rcProgress.top + _rcProgress.bottom) / 2 + 5,
		_rcProgress.right, _rcProgress.bottom);
}

void progressBar::setGauge(float currentGauge, float maxGauge)
{
    //퍼센테이지로 계산해주면 더욱 쉽고 조옿습니다
    _width = (currentGauge / maxGauge) * _progressBarBottom->getWidth();
}
