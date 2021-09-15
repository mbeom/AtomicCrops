#include "stdafx.h"
#include "timeManager.h"


timeManager::timeManager()
	: _timer(NULL)
{
}


timeManager::~timeManager()
{
}

HRESULT timeManager::init()
{
	_timer = new timer;
	_timer->init();

	world_time = 0;
	world_count = 0;
	day = 0;

	return S_OK;
}

void timeManager::release()
{
	if (_timer != nullptr)
	{
		SAFE_DELETE(_timer);
	}
}

//FPS 실질적으로 설정하는 곳
void timeManager::update(float lock)
{
	if (_timer != nullptr)
	{
		_timer->tick(lock);
	}

}

void timeManager::render()
{
#ifdef _DEBUG
	{
		if (_timer != nullptr)
		{
			WCHAR str2[128];
			swprintf_s(str2, L"FPS : %.1f", (float)_timer->getFrameRate());
			D2DMANAGER->DrawTextD2DCenter(D2DMANAGER->defaultBrush, L"메이플스토리", 17.0f, str2, 50, 50, 200, 150);

			swprintf_s(str2, L"WorldTime : %.1f", _timer->getWorldTime());
			D2DMANAGER->DrawTextD2DCenter(D2DMANAGER->defaultBrush, L"메이플스토리", 17.0f, str2, 50, 70, 200, 150);

			swprintf_s(str2, L"ElapsedTime : %.4f", _timer->getElapsedTime());
			D2DMANAGER->DrawTextD2DCenter(D2DMANAGER->defaultBrush, L"메이플스토리", 17.0f, str2, 50, 90, 250, 150);

		}
	}
#else
	{
		if (_timer != nullptr)
		{
			//프레임 출력
			wsprintf(str, "framePerSec : %d", _timer->getFrameRate());
			TextOut(hdc, 0, 0, str, strlen(str));
		}
	}
#endif
}

void timeManager::render(float x, float y)
{
	if (_timer != nullptr)
	{
		WCHAR str2[128];
		swprintf_s(str2, L"FPS : %.1f", (float)_timer->getFrameRate());
		D2DMANAGER->DrawTextD2DCenter(D2DMANAGER->defaultBrush, L"메이플스토리", 17.0f, str2, x, y, x + 200, y + 150);

		swprintf_s(str2, L"WorldTime : %.1f", _timer->getWorldTime());
		D2DMANAGER->DrawTextD2DCenter(D2DMANAGER->defaultBrush, L"메이플스토리", 17.0f, str2, x, y + 50, x + 200, y + 150);

		swprintf_s(str2, L"ElapsedTime : %.4f", _timer->getElapsedTime());
		D2DMANAGER->DrawTextD2DCenter(D2DMANAGER->defaultBrush, L"메이플스토리", 17.0f, str2, x, y + 100, x + 250, y + 150);

	}
}
